#ifndef DATATYPES_H
#define DATATYPES_H
struct SensorData {
    QString model_name;
    double start_angle;
    double arrive_angle;
    QString turn_direction;
};
struct UserType {
    QString username;
    QString type;  // prod // qual //admin //eng

};
#endif // DATATYPES_H
