MODULE Module1
    CONST robtarget Start:=[[541.140714835,67.292951727,121.5],[0,0.991444861,0.130526192,0],[-1,0,0,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
    CONST robtarget Target_10:=[[541.140753608,67.292957601,121.500009537],[0,0.991444864,0.130526175,0],[0,-1,0,1],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
    CONST robtarget Pick:=[[201.911,-381.869,-27.717],[0,-0.707106781,0.707106781,0],[-1,-1,0,1],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
    CONST robtarget AbovePick:=[[201.911,-381.869,121.5],[0,-0.707106781,0.707106781,0],[-1,-1,0,1],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
    CONST robtarget AbovePlace:=[[192.526,489.158,121.5],[0,-0.707106781,0.707106781,0],[0,1,0,1],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
    CONST robtarget Place:=[[192.526,489.158,-27.717],[0,-0.707106781,0.707106781,0],[0,1,0,1],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]];
    !***********************************************************
    !
    ! Module:  Module1
    !
    ! Description:
    !   <Insert description here>
    !
    ! Author: tn150
    !
    ! Version: 1.0
    !
    !***********************************************************


    !***********************************************************
    !
    ! Procedure main
    !
    !   This is the entry point of your program
    !
    !***********************************************************

    !Variables and constants
    CONST num x_increment:=50;
    CONST num y_increment:=50;
    VAR num x_pos:=0;
    VAR num y_pos:=0;
    VAR num my_pnp_height:=130;
    VAR pnpdata my_pnpdata;
    PROC main()
        !Add your code here
        Loop:
        !WorldAccLim\On:=30; !Acceleration limits to 30 m/s^2
        MoveL AbovePick,v2000,fine,tVacuumGripper\WObj:=wobj0;
!       MoveJ AbovePick,v2000,fine,tVacuumGripper\WObj:=wobj0;
        FOR y FROM 0 TO 2 DO
            !loop for collumn
            FOR x FROM 0 TO 2 DO
                !loop for row
                Path_10;
                x_pos:=x_pos-x_increment;
            ENDFOR
            y_pos:=y_pos-y_increment;
            x_pos:=0;
        ENDFOR
        y_pos:=0;
        WaitDI Entrance2,1;
        !Waiting for the sensor 2
        GOTO Loop;
    ENDPROC

    PROC Path_10()
        !Loop:
        my_pnpdata.smooth_start:=100;
        my_pnpdata.smooth_end:=100;
        SetDO Exit1,0;
        SetDO Exit2,0;
        WaitDI Entrance1,1;

!        MovePnP + MoveL         
        WaitTime 0.1; ! Wait Time 0.05s
        MoveLDO Pick,v2000,fine,tVacuumGripper\WObj:=wobj0,Exit1,1; !Pick the box out of the conveyor
        MovePnP Offs(Place,x_pos,y_pos,25),v2000,\PnPHeight:=my_pnp_height,fine,tVacuumGripper\WObj:=wobj0\PnPDataIN:=my_pnpdata \MaxTime:=1;
        SetDO Exit2,1;
        
        MoveL offs(AbovePlace,x_pos,y_pos,0),v2000,fine,tVacuumGripper\WObj:=wobj0;
        MovePnP AbovePick,v2000,\PnPHeight:=my_pnp_height,fine,tVacuumGripper\WObj:=wobj0\PnPDataIN:=my_pnpdata;


!        MovePnP + MoveJ         
       WaitTime 0.1; ! Wait Time 0.05s
       MoveJDO Pick,v2000,fine,tVacuumGripper\WObj:=wobj0,Exit1,1; !Pick the box out of the conveyor
       MovePnP Offs(Place,x_pos,y_pos,25),v2000,\PnPHeight:=my_pnp_height,fine,tVacuumGripper\WObj:=wobj0\PnPDataIN:=my_pnpdata \MaxTime:=1;
       SetDO Exit2,1;
        
       MoveJ offs(AbovePlace,x_pos,y_pos,0),v2000,fine,tVacuumGripper\WObj:=wobj0;
       MovePnP AbovePick,v2000,\PnPHeight:=my_pnp_height,fine,tVacuumGripper\WObj:=wobj0\PnPDataIN:=my_pnpdata;


!       MoveJ only        
       WaitTime 0.05; ! Wait time 0.05s
       MoveJDO Pick,v2000,fine,tVacuumGripper\WObj:=wobj0,Exit1,1; !Pick the box out of the conveyor
       MoveJ AbovePick,v2000,fine,tVacuumGripper\WObj:=wobj0;
        
       MoveJ offs(AbovePlace,x_pos,y_pos,0),v2000,fine,tVacuumGripper\WObj:=wobj0;
       WaitTime 0.05; ! Wait time 0.05s
       MoveJDO offs(Place,x_pos,y_pos,25),v2000,fine,tVacuumGripper\WObj:=wobj0,Exit2,1; !Place the box on the conveyor
       MoveJ offs(AbovePlace,x_pos,y_pos,0),v2000,fine,tVacuumGripper\WObj:=wobj0;
       MoveJ AbovePick,v2000,fine,tVacuumGripper\WObj:=wobj0;


!        MoveL + MoveJ
       WaitTime 0.05; ! Wait time 0.05s
       MoveLDO Pick,v2000,fine,tVacuumGripper\WObj:=wobj0,Exit1,1; !Pick the box out of the conveyor
       MoveL AbovePick,v2000,fine,tVacuumGripper\WObj:=wobj0;
        
       MoveJ offs(AbovePlace,x_pos,y_pos,0),v2000,fine,tVacuumGripper\WObj:=wobj0;
       WaitTime 0.05; ! Wait time 0.05s
       MoveLDO offs(Place,x_pos,y_pos,25),v2000,fine,tVacuumGripper\WObj:=wobj0,Exit2,1; !Place the box on the conveyor
       MoveL offs(AbovePlace,x_pos,y_pos,0),v2000,fine,tVacuumGripper\WObj:=wobj0;
       MoveJ AbovePick,v2000,fine,tVacuumGripper\WObj:=wobj0;

        !GOTO Loop;
    ENDPROC
ENDMODULE
