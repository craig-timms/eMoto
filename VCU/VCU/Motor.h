#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
    public:
        void setup( void );             // Done
        void restart( void );
        void service( void );

        void setMCUpower( bool );
        void setBrakeAN( bool );
        void setFoot( bool );
        void setThrottle( int );
        void setBrake( int );

//        int getThrottle( void )         { return throttleOUT; }      // returns (0-1000)
        int getRPM( void )              { return RPM; }
        int getCurrent( void )          { return currentMtr; }
        int getVoltage( void )          { return voltageDC; }
        int getTemp( void )             { return tempMtr; }
        int getTempInv( void )          { return tempInv; }
        int getState( void )            { return statusCmd; }
        // TODO: function to make sure set cmd == fb state
//        char getErrors( void )          { return ERR; }

    private:

        int throttleOUT = 0;
        int throttleRead = 0;
        int tempInv = 0;
        int tempMtr = 0;
        char statusInv[ 16 + 1 ];
        char statusCmd = 'N'; 
        char statusFb = 'N'; 
        int RPM = 0;
        int currentMtr = 0;
        int voltageDC = 0;
        char ERR[ 16 + 1 ];
        int HVstatus = 0;

};

#endif MOTOR_H
