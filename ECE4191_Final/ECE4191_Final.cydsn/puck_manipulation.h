/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef PUCK_MANIPULATION_H
    #define PUCK_MANIPULATION_H
    
    typedef enum {
        GripperUp,
        GripperDown
    } GripperUpOrDown;
    
    typedef enum {
        GripperOpen,
        GripperClose
    } GripperOpenOrClose;
    
    typedef enum {
        IROpen,
        IRClose
    } IROpenOrClose;
    
    typedef enum {
        ColorSensorOpen,
        ColorSensorClose
    } ColorSensorOpenOrClose;
    
    typedef enum {
        PuckNotOnRobot,   
        PuckOnRobot
    } PuckOnOrNotOnRobot;
    
    typedef enum {
        NotReachBase,  
        ReachBase
    } ProximityToBase;
    
    extern GripperUpOrDown gripperUpOrDown;
    extern GripperOpenOrClose gripperOpenOrClose;
    extern IROpenOrClose irOpenOrClose;
    extern ColorSensorOpenOrClose colorSensorOpenOrClose;
    extern PuckOnOrNotOnRobot puckOnOrNotOnRobot;
    extern ProximityToBase proximityToBase;
   
    void intializePuckManipulation();
    int setGripperArmStatus();
    int setGripperHandStatus();
    int setIRSensorStatus();
    int setColorSensorStatus();
    
    void setGripperArm();
    void setGripperHand();
    void setIRSensor();
    void setColorSensor();
    
    void startDetectColor();
    
    
#endif
/* [] END OF FILE */
