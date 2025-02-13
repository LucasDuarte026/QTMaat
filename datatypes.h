#ifndef DATATYPES_H
#define DATATYPES_H
struct SensorData {
    QString model_name;     // nome do modelo cadastrado do sensor
    double start_angle;     // Ângulo de inicio de sinal do sensor
    double arrive_angle;    // Ângulo de fim de sinal/curso do sensor
    QString turn_direction; // Direção de giro do sensor CW or CCW
};
struct UserType {
    QString username; // nome do usuário cadastrado no sistema
    QString type;  // Produção // Qualidade //Administrador //Engenharia
};


struct SafetyLimits {
    int emergencyStopTorque = 100;
    int overloadLevel = 70;
    int overspeedLevel = 600;
    double motorWorkingLimit = 0.1;
    int timeInterpolationPeriod = 4000;
    int speed = 0x16000000;
    int acceleration = 0x80000000;
    int deceleration = 0x80000000;
};

struct Homing {
    int switchSpeed = 8000000;
    int zeroSpeed = 8000000;
    int homingAcceleration = 33554432;
    int homingTorqueLimit = 500;
    int homingDetectionTime = 2048;
    int homingDetectionVelocity = 33554432;
    int communicationFunctionExtendedSetup = 40;
    int homingReturnSpeedLimit = 20000;
    int homingMode = 34;
    int touchprobeFunction = 7;
    int max_motor_speed = 500;  // rad/s
    int target_torque = 500;
    int max_torque = 600;
};

struct Positioning {
    int maxTorque = 500;
    int targetTorque = 500;
};

struct EngParameters {
    SafetyLimits safetyLimits;
    Homing homing;
    Positioning positioning;

    EngParameters() : safetyLimits(), homing(), positioning() {}
};

#endif // DATATYPES_H
