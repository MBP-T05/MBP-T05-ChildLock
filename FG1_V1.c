#include <stdio.h>

#define SIGNAL_OFF      0
#define SIGNAL_ON       1

/*
FG-01/ 도어 잠금/해제 제어
<Input>
1. SwitchInput : 차일드 락 시스템 활성화
2. VehicleSpeed : 차량 주행 속도(계기판)
3. CrashSignal : 비상 상황 등 이벤트로 시스템 해제해야 할 경우
4. RearDoorInnerHandleEvent : 뒷문 개폐 레버

<내부 변수>

1. IgnitionState  
2. DoorECUAck  
3. CurrentCLState : 차일드락 시스템 상태



<Output>
1. ChildLockCommand
2. OpenRequestBlock
3. HMIOutput
4. EventLog
5. DTC


*/


int FaultFlags = 0; // 의도 확인 필요
int RearDoor_Block = 0;

int InputMonitorAndValidator(int SwitchInput, int VehicleSpeed, int CrashSignal)
{
    int Switch_Err = 0;
    int Speedometer_Err = 0;
    int CrashSensor_Err = 0;
    int Err_detecting = 0;
    int EventFlags = 0;


    if(SwitchInput != SIGNAL_OFF && SwitchInput != SIGNAL_ON)
    {
        Switch_Err = 1; // Unstable Value
    }

    if(VehicleSpeed < 0)
    {
        Speedometer_Err = 1;
    }
 
    if(CrashSignal != SIGNAL_OFF && CrashSignal != SIGNAL_ON)
    {
        CrashSensor_Err = 1;
    }


    if(Switch_Err == 1 || Speedometer_Err == 1 || CrashSensor_Err == 1)
    {
        Err_detecting = 1;
    }
    return Err_detecting;
}

int RearDoor_Open_Event(int RearDoorInnerHandleEvent)
{
    if (RearDoorInnerHandleEvent == 1) return 1;
    else    return 0; 
}

int ChildLockStateDecision(int Err_Flags, int SwitchInput)
{
    int TargetCLState = 0;
    int SafetyAction = 0;

    if(Err_Flags == SIGNAL_ON){
        TargetCLState = 0; // 오류 혹은 차량 결함 발견 시 미연의 사고 방지 목적으로 기능 비활성화
        SafetyAction = 1;
        printf("차량 내 결함 검출로 차일드 락 기능을 제한합니다.\n");
    }
    else{
        TargetCLState = SwitchInput;
    }
    return TargetCLState ;
}


void RearDoorOpenBlockHandler(int CL_State, int RearDoorInnerHandleEvent)
{
    if(CL_State == SIGNAL_ON && RearDoorInnerHandleEvent == SIGNAL_ON){
            RearDoor_Block = 1;
            printf("경고 : 전자식 차일드 락 시스템이 활성화 중입니다.\n");
        
    }
    else if(CL_State == SIGNAL_OFF && RearDoorInnerHandleEvent == SIGNAL_ON)
    {
        RearDoor_Block = 0;
        printf("뒷문 열림\n");
    }

    else RearDoor_Block = 0;
}




int main()
{
    int SwitchInput = 0; // Main에서 조정
    int VehicleSpeed = 0;
    int CrashSignal = 0;
    int RearDoorInnerHandleEvent = 0;
    int CurrentCLState = 0;

    int Err_Flags = 0;
    int EventFlags = 0;

    int TargetCLState = 0;
    int SafetyAction = 0;

    char gear = 'p';


    Err_Flags = InputMonitorAndValidator(SwitchInput, VehicleSpeed, CrashSignal);
    CurrentCLState = ChildLockStateDecision(Err_Flags, SwitchInput);
    EventFlags = RearDoor_Open_Event(RearDoorInnerHandleEvent);

    printf("KIA\tMovement Inspires\n"); // 시동 on
    printf("주행 전 전자식 차일드 락 시스템의 활성화 여부를 확인해주세요 \n");
    scanf("%d", &CurrentCLState);

    if(CurrentCLState == 1)
    {
        printf("전자식 차일드 락 시스템이 활성화 되었습니다.\n");
    }
    else if(CurrentCLState == 0)
    {
        printf("전자식 차일드 락 시스템의 비활성화 상태를 유지합니다.\n");
    }
    else
    {
        printf("예기치 못한 값 발생\t 서비스 센터 방문 점검 필요\n");
    }


    printf("운전 모드(기어 위치)를 선택해주세요\n p : 주차, n : 중립, r : 후진, d : 주행\n");
    scanf("%c",&gear);
    switch(gear){
        case 'n':
            printf("n\n");
            break;
        case 'r':
            printf("r\n");
            break;
        case 'd':
            do{
                printf("주행을 종료하려면 기어의 위치를 옮겨주세요 : ");
                scanf("%c", &gear);
                printf("현재 차일드 락 시스템의 상태는 %d입니다. 1 : on, 0 : off\n 상태 변경을 원하신다면 1, 아니면 0을 눌러주세요. : ", CurrentCLState);
                int tmp = 0;
                scanf("%d", &tmp);
                if(tmp == 1){
                    if(CurrentCLState == 0){
                        CurrentCLState = 1;
                    }
                    else CurrentCLState = 0;
                }
                else{return 0;}
                tmp = 0;
                printf("뒷좌석 이벤트 발생을 원하시면 1, 아니면 다른 버튼을 눌러주세요. : ");
                scanf("%d", &tmp);
                if(tmp == 1){
                    RearDoorOpenBlockHandler(CurrentCLState, tmp);
                }
            }
            while(gear == 'd');
            break;
        case 'p':
            VehicleSpeed = 0;
        default:
            break;
    }

    
    // Assume : Vehicle is moving now . . .
    RearDoorOpenBlockHandler(CurrentCLState,RearDoorInnerHandleEvent);
    
    if(CrashSignal == 1){
        CurrentCLState = 0;
        printf("비상 상황 발생으로 차일드 락 기능을 강제 해제합니다.\n");
    }

}

