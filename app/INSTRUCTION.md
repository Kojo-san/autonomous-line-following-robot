# Projet final INF1900 - Robot autonome AVR

## Présentation

Ce dépôt contient le code d'un robot autonome développé dans le cadre du projet final du cours INF1900 à Polytechnique Montréal. Le robot est programmé pour parcourir une table en suivant un ruban noir, interpréter des choix de direction fournis avant le départ et adapter son trajet selon les obstacles détectés.

Le projet met en pratique les notions de programmation embarquée, d'intégration de capteurs, de contrôle moteur et de conception logicielle sur microcontrôleur AVR.

## Objectifs du robot

Le robot doit notamment être capable de :

- suivre une ligne noire sur un parcours prédéfini;
- recevoir avant le départ des consignes de direction aux points clés du trajet;
- confirmer les choix de l'évaluateur à l'aide d'une DEL bicolore;
- détecter les obstacles, poteaux et zones particulières du parcours;
- gérer les intersections et la section de grille;
- compléter le parcours en respectant le comportement attendu;
- signaler la fin de l'exécution par un clignotement de la DEL.

## Organisation du dépôt

Le dépôt est organisé autour de deux dossiers principaux :

- `lib` : bibliothèque bas niveau regroupant les modules matériels et utilitaires, comme la gestion des moteurs, des capteurs, de la DEL, des timers, de la communication série, du CAN et des interruptions.
- `app` : logique applicative principale du robot, incluant le point d'entrée `main.cpp` et l'orchestration générale du comportement.

## Technologies et composants

Le projet utilise principalement :

- C/C++ embarqué;
- AVR-GCC;
- microcontrôleur ATmega324PA;
- fréquence d'horloge `F_CPU = 8 MHz`;
- capteurs de ligne;
- capteurs infrarouges ou de distance;
- moteurs contrôlés par PWM;
- DEL bicolore;
- timers matériels;
- interruptions externes;
- bus CAN lorsque le module est utilisé par le projet.

## Compilation

La compilation est prévue avec AVR-GCC et les Makefiles fournis dans le dépôt.

Depuis le dossier `lib`, compiler d'abord la bibliothèque :

```sh
make
```

Depuis le dossier `app`, compiler ensuite l'application :

```sh
make
```

Les fichiers générés sont destinés au microcontrôleur AVR cible. Pour nettoyer les sorties de compilation :

```sh
make clean
```

à exécuter dans les dossiers `lib` et `app` selon le besoin.

## Téléversement

Après compilation, le programme peut être téléversé sur le robot avec l'outil prévu par l'environnement AVR, par exemple `avrdude`.

Le Makefile de l'application fournit aussi une cible d'installation :

```sh
make install
```

Cette commande suppose que le programmeur matériel est connecté et correctement configuré.

## Comportement attendu

Avant le départ, l'évaluateur sélectionne les directions à prendre sur le parcours à l'aide des boutons du robot. La DEL bicolore confirme les choix effectués.

Après la phase de sélection et un court délai de démarrage, le robot commence son trajet. Il suit le ruban noir, détecte les intersections, réagit aux obstacles et adapte son déplacement dans les zones contenant des poteaux ou une grille.

À la fin du parcours, le robot doit s'arrêter et signaler la fin de l'exécution par un clignotement alterné de la DEL rouge et verte.

