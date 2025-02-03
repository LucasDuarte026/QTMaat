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
#endif // DATATYPES_H
