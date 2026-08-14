//
// Created by 13466 on 2026/8/13.
//

#include "chassis.h"
#include "robot_def.h"
#include "power_control.h"
#include "super_cap.h"
#include "message_center.h"
#include "referee_task.h"

#include "general_def.h"
#include "bsp_dwt.h"
#include "referee_UI.h"
#include "arm_math.h"
#define HALF_WHEEL_BASE (WHEEL_BASE /2) //半轴距
#define HALF_TRACK_WIDTH (TRACK_WIDTH /2.0f) //半轮距
#define PERIMETER_WHEEL (RADIUS_WHEEL * 2 * PI) // 轮子周长


static Subscriber_t *chassis_sub;
static Publisher_t *chassis_pub;
static Chassis_Ctrl_Cmd_s chassis_cmd_rcv;
static Chassis_Upload_Data_s gimbal_upload_data;

static SuperCapInstance *cap;
static DJIMotorInstance *motor_lf,*motor_rf,*motor_lb,*motor_rb;

static float chassis_vx, chassis_vy;
static float vt_lf, vt_rf, vt_lb, vt_rb;                  // 底盘速度解算后的临时输出,待进行限幅

void chassiInit()
{
    Motor_Init_Config_s chassis_motor_config ={
        .can_init_config.can_handle = &hcan1,
        .controller_param_init_config = {
        .speed_PID = {
        .Kp = 0.0,
            .Ki = 0.0,
            .Kd = 0.0,
            .IntegralLimit = 100000,
            .Improve = PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement,
            .MaxOut=1000,
            .Output_LPF_RC = 0.3,
            },
        },
            .controller_setting_init_config = {
                .angle_feedback_source = MOTOR_FEED,
                .speed_feedback_source = MOTOR_FEED,
                .outer_loop_type = SPEED_LOOP, // 设置为开环，电机设定值由下面的功率控制设定，不走普通的pid
                .close_loop_type = SPEED_LOOP,
            },
            .motor_type = M3508,

    };
    chassis_motor_config.can_init_config.tx_id =1;
    chassis_motor_config.controller_setting_init_config.motor_reverse_flag =MOTOR_DIRECTION_NORMAL;
    motor_lf = PowerControlInit(&chassis_motor_config);

    chassis_motor_config.can_init_config.tx_id = 2;
    chassis_motor_config.controller_setting_init_config.motor_reverse_flag =MOTOR_DIRECTION_NORMAL;
    motor_rf = PowerControlInit(&chassis_motor_config);

    chassis_motor_config.can_init_config.tx_id = 3;
    chassis_motor_config.controller_setting_init_config.motor_reverse_flag =MOTOR_DIRECTION_NORMAL;
    motor_lb = PowerControlInit(&chassis_motor_config);

    chassis_motor_config.can_init_config.tx_id = 4;
    chassis_motor_config.controller_setting_init_config.motor_reverse_flag =MOTOR_DIRECTION_NORMAL;
    motor_rb = PowerControlInit(&chassis_motor_config);

    chassis_sub = SubRegister("chassis_cmd",sizeof(Chassis_Ctrl_Cmd_s));
    chassis_pub = PubRegister("chassis_feed",sizeof(Chassis_Upload_Data_s));

}


static void MecanumCalculate()
{



}
void chassisTask()
{


}