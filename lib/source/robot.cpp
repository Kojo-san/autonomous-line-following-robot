#include "robot.h"

Robot::Robot() 
    : led(),
      motor(),
      sound(),
      lineSensor(),
      motion(motor, lineSensor),
      distanceSensor()
    {motor.init();
    distanceSensor.init();
    sound.init();
    }

void Robot::init() {
    ExternalInterrupts int0(InterruptNumber::INTERRUPT0, InterruptMode::BOTH_EDGES);
    int0.enableInterrupt();
    ExternalInterrupts int1(InterruptNumber::INTERRUPT1, InterruptMode::BOTH_EDGES);
    int1.enableInterrupt();
    sei();
}

uint8_t Robot::getFirstButton() const { return firstButton; }

uint8_t Robot::getSecondButton() const { return secondButton; }





void Robot::handleButtonPress(uint8_t buttonState, uint8_t buttonValue, void (Led::*ledAction)()) {
    _delay_ms(BUTTON_DEBOUNCE_MS);

    if (buttonState && pressCount < BUTTON_PRESS_TARGET) {
        (led.*ledAction)();
        if (pressCount == 0) {
            firstButton = buttonValue;
        } else {
            secondButton = buttonValue;
        }
    }
    else if (!buttonState) {
        pressCount++;
        led.off();
    
    }
}

void Robot::handleBC() {
    wait();  // attendre 2 appuis

    uint8_t dirB = getFirstButton();
    uint8_t dirC = getSecondButton();
   
    // Point B
    motor.adjustSpeed(125,150);
    Timer1::waitDelay_ms(600);
    handleButtonAction(dirB);

    // Point C
    motion.followLineUntil(0);
    motor.adjustSpeed(125,150);
    Timer1::waitDelay_ms(600);
    handleButtonAction(dirC);

    // Partie 3 – revenir à la ligne principale
    motor.adjustSpeed(125,150);
    Timer1::waitDelay_ms(400);
    motion.followLineUntil(0);  // suivre la branche après C

    // Faire le virage inverse à celui du point C
    motor.adjustSpeed(125,150);
    Timer1::waitDelay_ms(400);
    if (dirC == 0) {  // gauche à C → maintenant tourner à droite
        while (!lineSensor.S3) {
            lineSensor.updateState();
            motor.adjustSpeed(100, 0);  // droite
        }
    } else {  // droite à C → maintenant tourner à gauche
        while (!lineSensor.S3) {
            lineSensor.updateState();
            motor.adjustSpeed(0, 100);  // gauche
        }
    }

    motor.stop();
    Timer1::waitDelay_ms(500);

    // Continuer le parcours
    motion.followLineUntil(0);

    motion.turn90RightCaptor();
}

void Robot::wait() {
    while (pressCount < BUTTON_PRESS_TARGET) {
        
    }
    _delay_ms(START_WAIT_DELAY_MS);
}

void Robot::playSound() {
    motor.stop();
    sound.playSound(LOW_NOTE);
    Timer1::waitDelay_ms(1000);
    sound.reset();
}

void Robot::handleButtonAction(uint8_t button) {
    playSound();
    // Étape 2 : tourne à gauche ou à droite jusqu’à retrouver la ligne (S3 détecté)
    if (button == 0) {  // bouton blanc appuyé = gauche
        while (!lineSensor.S3) {
            lineSensor.updateState();
            motor.adjustSpeed(0, 100); 
        }
    } else if (button == 1) {  // bouton interrupt appuyé = droite
        while (!lineSensor.S3) {
            lineSensor.updateState();
            motor.adjustSpeed(100, 0);  // tourner à droite
        }
    }

    // Étape 3 : stabilisation
    motor.stop();
    Timer1::waitDelay_ms(500);
}
      

void Robot::endLed() {
    motor.stop();
    while (true) {
        led.red();
        _delay_ms(LED_BLINK_DELAY_MS);
        led.green();
        _delay_ms(LED_BLINK_DELAY_MS);
    }
}

void Robot::indicateObstacle() {
    motor.stop();
    for (uint8_t i = 0; i < 8; i++) {
        led.red();
        _delay_ms(LED_BLINK_DELAY_MS);
        led.off();
        _delay_ms(LED_BLINK_DELAY_MS);
    }
}

void Robot::indicateGoodPassage() {
    motor.stop();
    for (uint8_t i = 0; i < 8; i++) {
        led.green();
        _delay_ms(LED_BLINK_DELAY_MS);
        led.off();
        _delay_ms(LED_BLINK_DELAY_MS);
    }  
}

bool Robot::detectHouse() {
    return distanceSensor.detectHouse();
}

bool Robot::detectFirstPoleGrid() {
    return distanceSensor.isObstacle();
}

bool Robot::detectSecondPoleGrid() {
    return distanceSensor.isObstacle();
}

void Robot::testDistanceSensor() {
    distanceSensor.enableReadValues();
    distanceSensor.detectObstacle();
}

void Robot::testCaptorDistance() {
    testDistanceSensor();
}



void Robot::houseLoop() {
    uint8_t crossingCount = 0;
    bool detectionArmed = true;
    uint8_t lostLineCount = 0;
    while (true) {
        lineSensor.updateState();
        motion.followLine();

        // Détection de croisement
        if ((lineSensor.sensorsDetected == 3 || lineSensor.sensorsDetected == 4) && detectionArmed) {
            crossingCount++;
            led.green();
            Timer1::waitDelay_ms(800);
            led.off();
            detectionArmed = false;
        }

        // Réarmer la détection une fois quitté
        if (lineSensor.sensorsDetected < 3) {
            detectionArmed = true;
        }

        // Si on a franchi 2 croisements → on entre dans la logique de la grille
        if (crossingCount == 2) {
            motor.stop();
            Timer1::waitDelay_ms(500);
            motor.adjustSpeed(0, 120);
            Timer1::waitDelay_ms(1300);
            lineSensor.updateState();
            motion.moveBackwardLeft(135);
            motor.adjustSpeed(125,150);
            Timer1::waitDelay_ms(200);

            bool obstacleDetected = false;

            while (true) {
                lineSensor.updateState();
                motion.followLine();

                if (distanceSensor.detectHouse()) {
                    motor.stop();
                    obstacleDetected = true;
                    break;
                }

                if (lineSensor.sensorsDetected == 0) {
                    lostLineCount++;
                    if (lostLineCount > 40)
                        break;
                } else {
                    lostLineCount = 0;
                }

                Timer1::waitDelay_ms(10);
            }

            motor.stop();
            Timer1::waitDelay_ms(500);

            if (obstacleDetected) {
                indicateObstacle();
                motion.turn90RightCaptorHouse();
                motion.followLineUntil(0);
                motion.turnRightUntil();
                motion.followLineUntil(0);
                led.red();
                Timer1::waitDelay_ms(500);
                motion.turn90RightCaptor();
                led.green();
                Timer1::waitDelay_ms(500);
                motion.followLineUntil(0);
                led.red();
                Timer1::waitDelay_ms(500);
                motion.turn90RightCaptor();
                break;
            } else {
                indicateGoodPassage();
                motor.moveForward(120);
                Timer1::waitDelay_ms(1000);
                motion.moveBackwardLeft(110);
                motion.followLineUntil(0);
                motion.turn90RightCaptor();
                led.red();
                Timer1::waitDelay_ms(500);
                motion.followLineUntil(0);
                led.green();
                Timer1::waitDelay_ms(500);
                motion.turn90RightCaptor();
                led.red();
                Timer1::waitDelay_ms(500);
                break;
            }
        }

        Timer1::waitDelay_ms(10);
    }
}

       

          
     




         
void Robot::fromL() {
    bool obstacleDetectedInPath = motion.followLineUntilPoteau(distanceSensor);
    
    if (!obstacleDetectedInPath) {
        Timer1::waitDelay_ms(300); // Stabilisation indispensable
        lineSensor.updateState(); // Actualisation de l'état
        motion.turn90RightCaptor(); // Rotation unique
        motion.followLineUntil(0);
        Timer1::waitDelay_ms(1000); // Stabilisation indispensable
        motion.turn90RightCaptor();
        Timer1::waitDelay_ms(1000); // Stabilisation indispensable

    } else {
        motor.stop();
        Timer1::waitDelay_ms(1300);
        motion.moveBackwardLeft(105); 
        motion.followLineUntil(0);
        motion.turn90LeftCaptor(); // Rotation unique pour sortir du L
        
        bool obstacleDetectedAtN = motion.followLineUntilPoteau(distanceSensor);
        
        if (!obstacleDetectedAtN) {
            Timer1::waitDelay_ms(300); // Stabilisation indispensable
            lineSensor.updateState(); // Actualisation de l'état
            motion.turn90RightCaptor(); // Rotation unique vers nouvelle direction
            motion.followLineUntil(0);
            motion.turn90RightCaptor();
        } else {
            motion.moveBackwardRight(120);
            motion.followLineUntil(0);
            motion.turn90RightCaptor(); // Rotation unique de repli
        }
    }
}

void Robot::fromK() {
    
    bool obstacleDetectedInPath = motion.followLineUntilPoteau(distanceSensor);
    
    if (!obstacleDetectedInPath) {
        motion.turn90RightCaptor();
        motion.followLineUntil(0);
        motion.turn90RightCaptor();


    } else {
        motor.stop();
        Timer1::waitDelay_ms(1300);
        motion.moveBackwardRight(105);
        motor.stop();
        Timer1::waitDelay_ms(300);
        
        motion.followLineUntil(0);
        motion.turn90RightCaptor();
        
        
        
        bool obstacleDetectedAtN = motion.followLineUntilPoteau(distanceSensor);
        
        if (!obstacleDetectedAtN) {
            motion.turn90LeftCaptor();
            motion.followLineUntil(0);
            motion.turn90RightCaptor();
            motion.followLineUntil(0);
            motion.turn90RightCaptor();
        } else {
            motion.moveBackwardLeft(120);
            motion.followLineUntil(5);
            motor.moveForward(120);
            Timer1::waitDelay_ms(1100);
            motor.adjustSpeed(100,0);
            Timer1::waitDelay_ms(1100);
            lineSensor.updateState();
            motion.moveBackwardRight(120);
            motion.followLineUntil(0);
            motion.turn90RightCaptor();
            motion.followLineUntil(0);
            motion.turn90RightCaptor();
            
        }   
    }
}

void Robot::fromM() {
    bool obstacleDetectedInPath = motion.followLineUntilPoteau(distanceSensor);
    
    if (!obstacleDetectedInPath) {
        motion.turn90RightCaptor();
        motion.followLineUntil(0);
        motion.turn90RightCaptor();
        

    } else {
        motor.stop();
        Timer1::waitDelay_ms(1300);
        
        motion.moveBackwardLeft(105);
        motion.followLineUntil(0);
        motion.turn90LeftCaptor();
        
        bool obstacleDetectedAtN = motion.followLineUntilPoteau(distanceSensor);
        
        if (!obstacleDetectedAtN) {
            motion.turn90RightCaptor();
            motion.followLineUntil(0);
            motion.turn90RightCaptor();


            
        } else {
            motion.moveBackwardRight(120);
            motion.followLineUntil(5);
            motor.moveForward(120);
            Timer1::waitDelay_ms(1100);
            motor.adjustSpeed(0,100);
            Timer1::waitDelay_ms(1100);
            lineSensor.updateState();
            motion.moveBackwardLeft(120);
            motion.followLineUntil(0);
            motion.turn90RightCaptor();
            motion.followLineUntil(0);
            motion.turn90RightCaptor();

        }
    }
}


void Robot::gridStateNavigation() {
    GridState state = GridState::FOLLOW_INITIAL_LINE;
    bool obstacleDetected = false;
    bool obstacleDetectedAgain = false;
    uint8_t fullDetectionCount = 0;
    bool alreadyPassed = false;
    
    while (state != GridState::END) {
        switch (state) {
        case GridState::FOLLOW_INITIAL_LINE:
            obstacleDetected = motion.followLineUntilPoteau(distanceSensor);
            state = GridState::CHECK_FIRST_OBSTACLE;
            break;
            
        case GridState::CHECK_FIRST_OBSTACLE:
            fullDetectionCount = 0;
            alreadyPassed = false;
            while (true) {
                lineSensor.updateState();
                motion.followLine();
                if (lineSensor.sensorsDetected == 5) {
                    if (!alreadyPassed) {
                        fullDetectionCount++;
                        alreadyPassed = true;
                        Timer1::waitDelay_ms(300);
                    }
                } else {
                    alreadyPassed = false;
                }
                if (fullDetectionCount == 1) {
                    fromL();
                    state = GridState::END;
                    break;
                }
                if (obstacleDetected) {
                    motor.stop();
                    Timer1::waitDelay_ms(500);
                    led.red();
                    Timer1::waitDelay_ms(500);
                    led.off();
                    state = GridState::RETREAT_AND_TURN;
                    break;
                }
                Timer1::waitDelay_ms(10);
            }
            break;
            
        case GridState::RETREAT_AND_TURN:
            motion.moveBackwardLeftGrid(105);
            motor.stop();
            Timer1::waitDelay_ms(500);
            motion.followLineUntil(0);
            motion.turn90LeftCaptor();
            state = GridState::CHECK_SECOND_OBSTACLE;
            break;
            
        case GridState::CHECK_SECOND_OBSTACLE:
            obstacleDetectedAgain = motion.followLineUntilPoteau(distanceSensor);
            state = GridState::EVALUATE_K_POSITION;
            break;
            
        case GridState::EVALUATE_K_POSITION:
            fullDetectionCount = 0;
            alreadyPassed = false;
            while (true) {
                lineSensor.updateState();
                motion.followLine();
                if (lineSensor.sensorsDetected == 5) {
                    if (!alreadyPassed) {
                        fullDetectionCount++;
                        alreadyPassed = true;
                        Timer1::waitDelay_ms(300);
                    }
                } else {
                    alreadyPassed = false;
                }
                if (fullDetectionCount ==  1) {
                    fromK();
                    state = GridState::END;
                    break;
                }
                if (obstacleDetectedAgain) {
                    led.red();
                    Timer1::waitDelay_ms(500);
                    led.off();
                    state = GridState::FALLBACK_TO_M;
                    break;
                }
                Timer1::waitDelay_ms(10);
            }
            break;
            
        case GridState::FALLBACK_TO_M:
            motion.moveBackwardRightGrid(120);
            motion.followLineUntil(0);
            motion.turn90RightCaptor();
            fromM();
            state = GridState::END;
            break;
            
        case GridState::END:
            break;
        }
    }
}
