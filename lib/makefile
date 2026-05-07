########       AVR Project Makefile       ########
#####                                        #####
#####      Makefile produit et ecrit par     #####
#####   Simon Barrette & Jerome Collin pour  #####
#####           INF1900 - 2016               #####
#####                                        #####
#####         Inspire de Pat Deegan -        #####
#####  Psychogenic Inc (www.psychogenic.com) #####
##################################################

# Ce Makefile vous permet de compiler des projets
# pour les microcontroleurs Atmel AVR sur 
# Linux ou Unix, en utilisant l'outil AVR-GCC. 
# Ce Makefile supporte C & C++

#####      Details specifique a la cible       #####
#####  Vous devez les adapter a votre projet   #####

# Nom du microcontroleur cible
MCU=atmega324pa

# Répertoires du projet
INCDIR=include
SRCDIR=source
OBJDIR=obj

# Nom du projet
PROJECTNAME=libmonprojet


# Recherche des fichiers sources (.cpp)
SRCS=$(wildcard $(SRCDIR)/*.cpp)

# Génération des fichiers objets correspondants
OBJS=$(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Inclusions additionnelles
INC=-I$(INCDIR)

# Niveau d'optimisation
OPTLEVEL=s

# Generation du fichier .a
LIBRARY=$(PROJECTNAME).a 

# Programmer ID - Ne pas changer 
# Liste complete des IDs disponible avec avrdude
AVRDUDE_PROGRAMMERID=usbasp



####################################################
#####         Configuration terminee           #####
#####                                          #####
#####  Le reste de cette section contient les  #####
##### details d'implementation vous permettant #####
##### de mieux comprendre le fonctionnement de #####
#####   ce Makefile en vue de sa modification  #####
####################################################



####### variables #######

#compilateur utilise
CC=avr-gcc
#pour generer la bibliotheque statique AVR
AR=avr-ar
#pour copier le contenu d'un fichier objet vers un autre
OBJCOPY=avr-objcopy
#pour permettre le transfert vers le microcontroleur
AVRDUDE=avrdude
#pour supprimer les fichiers lorsque l'on appel make clean
REMOVE=rm -f
# HEXFORMAT -- format pour les fichiers produient .hex
HEXFORMAT=ihex

CFLAGS=-I. -I/usr/include/simavr -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-std=c++14 -fshort-enums \
	-funsigned-bitfields -funsigned-char \
	-Wall

CXXFLAGS=-fno-exceptions

GCCVERSION := $(shell expr `$(CC) -dumpversion | cut -f1 -d.` \>= 12)
ifeq "$(GCCVERSION)" "1"
    CFLAGS += --param=min-pagesize=0
endif

.PHONY: all clean

# Compilation complète
all: $(LIBRARY)

# Compilation de la bibliothèque statique
$(LIBRARY): $(OBJS)
	$(AR) rcs $@ $^

# Compilation des fichiers objets
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@


# Création du dossier des objets si inexistant
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Nettoyage
clean:
	rm -rf $(OBJDIR) $(LIBRARY) *.d
