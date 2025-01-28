/** \file
 * \brief Control Minas A6B into 2 functions
 *
 *  Usage:  setPos -s                               - For homing operation until arrive zero position
 *          setPos -p position velocity             - For position operation until a absolute position beyond zero reference
 *  ifname is NIC interface, f.e. eth0
 *
 */

#include <stdio.h>            // controle de saída base std com o usuário
#include <string.h>           // controle de strings
#include "ethercat_manager.h" // bilbioteca base, usada pelo minas, para comunicação EtherCAT
#include "minas_client.h"     // Biblioteca especifica do minas a6b
#include <time.h>             // usada para controle de tempo dentro do código

static const int NSEC_PER_SECOND = 1e+9;
static const int USEC_PER_SECOND = 1e+6;

// declaração das funções a serem usadas aqui neste contexto
void ppMode(minas_control::MinasClient *client, minas_control::MinasOutput output, minas_control::MinasInput input);
void hmMode(minas_control::MinasClient *client, minas_control::MinasOutput output, minas_control::MinasInput input);
void help();
void timespecInc(struct timespec &tick, int nsec);

// função principal, capaz de mandar o servo motor para a posição home e para qualquer posição absoluta (relativa ao zero)
int main(int argc, char *argv[])
{
    int operation_mode = 0;        // Será a variavel base para armazenar o tipo de operação a ser feita. 0x01 para pp e 0x06 para homing
    std::string ifname = "enp2s0"; // Caso em linux, usa-se esse adaptador, caso outra plataforma, como windows (eth0): adaptar

    printf("\n -- -- -- -- -- -- -- -- Projeto Ma'at -- -- -- -- -- -- -- -- \n");
    printf("\n  Algorítmo destinado ao controole do driver Minas A6-B via comunicação EtherCAT\n");
    printf("\n  funções: Homing e positional\n");

    if (argc > 1) // análise da entrada de comandos e seleção de modo
    {
        if (strcmp(argv[1], "-h") == 0)
        { //  Caso o usuário opte para saber como funciona o código
            help();
            exit(0);
        }
        else if (strcmp(argv[1], "-p") == 0)
        {
            operation_mode = 0x01; // (pp) position profile mode
        }
        else if (strcmp(argv[1], "-s") == 0)
        {
            operation_mode = 0x06; // (hm) homing mode
        }
        printf("    ->  Operation_mode: %d \n", operation_mode);
    }
    else // caso não tenha-se argumento algum, instrui ao usuário como usar
    {
        help();
        exit(0);
    }

    try
    {
        /* start slaveinfo */
        ethercat::EtherCatManager manager(ifname);
        std::vector<minas_control::MinasClient *> clients;
        for (int i = 0; i < manager.getNumClients(); i++)
        {
            // Após cirar o vetr de clientes, passa-se por todos eles dando push_back através da biblioteca SOEM
            clients.push_back(new minas_control::MinasClient(manager, i + 1));
        }

        for (std::vector<minas_control::MinasClient *>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            // Depois de criados, passa-se um interador (it) para cada cliente na rede EtherCAT
            minas_control::MinasClient *client = (*it);
            // Limpa quaisquer erro ativo no sensor
            client->reset();

            // Configurações gerais de limites operacionais para segurança do equipamento sob uso
            client->setTrqueForEmergencyStop(100);    // 100%  - uso do torque para parada de emergência      (No. SX-DSV03242 - 251 - EtherCAT Communication Specification)
            client->setOverLoadLevel(70);             // 50%   - limite de torque para parada de emergência   (No. SX-DSV03242 - 338 - EtherCAT Communication Specification)
            client->setOverSpeedLevel(600);           // r/min - limite de velocidade para uso  comum         (No. SX-DSV03242 - 338 - EtherCAT Communication Specification)
            client->setMotorWorkingRange(0.1);        // 0.1 -   Limite de operação do motor                  (No. SX-DSV03241 - 155 - Functional Specification)
            client->setInterpolationTimePeriod(4000); // 4 msec                                            (No. SX-DSV03242 - 65  -EtherCAT Communication Specification)

            // servo on
            client->servoOn(operation_mode); //   No. SX-DSV03242 - 11, 13 e 26 -  EtherCAT Communication Specification

            // Pegar a posição atual após ligar o equipamento
            minas_control::MinasInput input = client->readInputs(); // input no formato padronizado
            int32 current_position = input.position_actual_value;   // pegar a posição vinda do

            // output a ser enviado a depender da configuração de operação escolhida
            minas_control::MinasOutput output;
            memset(&output, 0x00, sizeof(minas_control::MinasOutput));

            // (pp) position profile mode
            if (operation_mode == 0x01)
            {
                double entrance = std::stod(argv[2]);             // entrance é umm valor que remete a revolução em double. 2 = 2 revoluções; 0.5 = meia revolução
                output.target_position = 0 + 0x800000 * entrance; // 0x800000 é equivalente à uma revolução
            }

            output.max_motor_speed = (argc > 2 && operation_mode == 0x01) ? std::stod(argv[3]) : 500; // rad/min Velocidade do motor definida na entrada. 500 como padrão. 600 como limite acima
            output.target_torque = 500;                                                               // 0% (unit 0.1%)           torque usado, 50% é suficiente
            output.max_torque = 500;                                                                  // 50% (unit 0.1%)          Torque máximmo também é 50%

            /*  pp mode: move to operation enabled + new-set-point (bit4) + change set immediately (bit5)
                hm mode: move to operation enabled + start hmoming
              */
            output.controlword = 0x001f; // Procurar as definições de controlWord para os métodos pp e hm (são diferentes para cada)

            output.operation_mode = operation_mode; // definide de fato o modo de operação para a escrita

            // configurar as acelerações e velocidades de perfil. Não alterar
            client->setProfileVelocity(0x16000000);
            client->setProfileAcceleration(0x80000000);
            client->setProfileDeceleration(0x80000000);

            // configurar os as aplicações de fato
            if (operation_mode == 0x01)
                ppMode(client, output, input);

            else if (operation_mode == 0x06)
                hmMode(client, output, input);
        }

        /*



                -------------------------------------------- Acompanhar requisição ----------------------------------------------------------------



                Aṕos o comando ser enviado, o servo irá trabalhar e o for a seguir tende a seguir e ler essa operação até, fazendo um print de controle
        */

        // ticks usados para esperar a cada  4ms in nanoseconds para leitura e interação com o drive
        double period = 4e+6;
        // pegar o tempo atual
        struct timespec tick;
        clock_gettime(CLOCK_REALTIME, &tick);
        // inicializar o tick (contador de tempo)
        timespecInc(tick, period);

        printf("\n\t->\tAcompanhar operação\n");
        for (int i = 0; i <= 2000; i++)
        {
            for (std::vector<minas_control::MinasClient *>::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                // Fazer uma leitura para cada drive conectado
                minas_control::MinasClient *client = (*it);
                
                // ler o estado do minas
                minas_control::MinasInput input = client->readInputs();
                minas_control::MinasOutput output = client->readOutputs();

                if (i % 10 == 0)
                {// A cada  interações, mostrar o estado
                    printf("err = %04x, ctrl %04x, status %04x, op_mode = %2d, pos = %08x, vel = %08x, tor = %08x\n",
                           input.error_code, output.controlword, input.statusword, input.operation_mode, input.position_actual_value, input.velocity_actual_value, input.torque_actual_value);
                    if (input.statusword & 0x0400)
                    { // target reached (bit 10)
                        printf("target reached\n"); // caso chegue antes das interações 
                        break;
                    }
                    printf("Tick %8ld.%09ld\n", tick.tv_sec, tick.tv_nsec);
                    printf("Input:\n");
                    printf(" 603Fh %08x :Error code\n", input.error_code);
                    printf(" 6041h %08x :Statusword\n", input.statusword);
                    printf(" 6061h %08x :Modes of operation display\n", input.operation_mode);
                    printf(" 6064h %08x :Position actual value\n", input.position_actual_value);
                    printf(" 606Ch %08x :Velocity actual value\n", input.velocity_actual_value);
                    printf(" 6077h %08x :Torque actual value\n", input.torque_actual_value);
                    printf(" 60B9h %08x :Touch probe status\n", input.touch_probe_status);
                    printf(" 60BAh %08x :Touch probe pos1 pos value\n", input.touch_probe_posl_pos_value);
                    printf(" 60FDh %08x :Digital inputs\n", input.digital_inputs);
                    printf("Output:\n");
                    printf(" 6040h %08x :Controlword\n", output.controlword);
                    printf(" 6060h %08x :Mode of operation\n", output.operation_mode);
                    printf(" 6071h %08x :Target Torque\n", output.target_torque);
                    printf(" 6072h %08x :Max Torque\n", output.max_torque);
                    printf(" 607Ah %08x :Target Position\n", output.target_position);
                    printf(" 6080h %08x :Max motor speed\n", output.max_motor_speed);
                    printf(" 60B8h %08x :Touch Probe function\n", output.touch_probe_function);
                    printf(" 60FFh %08x :Target Velocity\n", output.target_velocity);
                    printf(" 60B0h %08x :Position Offset\n", output.position_offset);
                }

            } // for clients

            // usleep(4*1000);
            timespecInc(tick, period);
            // check overrun
            struct timespec before;
            clock_gettime(CLOCK_REALTIME, &before);
            double overrun_time = (before.tv_sec + double(before.tv_nsec) / NSEC_PER_SECOND) - (tick.tv_sec + double(tick.tv_nsec) / NSEC_PER_SECOND);
            if (overrun_time > 0.0)
            {
                fprintf(stderr, "  overrun: %f", overrun_time);
            }
            clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tick, NULL); // esperar o prox tick
        }

        for (std::vector<minas_control::MinasClient *>::iterator it = clients.begin(); it != clients.end(); ++it)
        { // finalizar  a operação do driver
            minas_control::MinasClient *client = (*it);
            minas_control::MinasInput input = client->readInputs();
            client->printPDSStatus(input);
            client->printPDSOperation(input);
            client->servoOff();
        }
    }
    catch (...)
    {
        help();
    }

    printf("End program\n");

    return 0;
}

void timespecInc(struct timespec &tick, int nsec)
{
    tick.tv_nsec += nsec;
    while (tick.tv_nsec >= NSEC_PER_SECOND)
    {
        tick.tv_nsec -= NSEC_PER_SECOND;
        tick.tv_sec++;
    }
}

void help()
{
    fprintf(stderr, "  \nAvailable options\n");
    fprintf(stderr, "    -s , --homing mode  This command seach for the absolute zero and set it\n");
    fprintf(stderr, "    -p value1 value2,  --position that put the servo in a desired position with velocity control\n");
    fprintf(stderr, "       ->  value1: double input value: revolutions in offset of zero point\n");
    fprintf(stderr, "       ->  value2: velocity in rad/s of the servo motor (100 as standard)\n");
    fprintf(stderr, "    -h, --help          Print this message and exit\n\n");
}

void hmMode(minas_control::MinasClient *client, minas_control::MinasOutput output, minas_control::MinasInput input)
{
    printf("\t->\thm mode init\n");


    // todas as seguinte configurações seguem o indicado pela documentação para parametrização do homing. sua velocida, aceleração e método
    // Confira todo o capitulo: 6-6-5 Homing Position Mode (hm mode) - No. SX-DSV03242 - 137 - EtherCAT Communication Specification)

    client->setSwitchSpeed(8000000); 
    client->setZeroSpeed(8000000);
    client->setHomingAcceleration(33554432);
    client->setHomingTorqueLimit(500);
    client->setHomingDetectionTime(2048);
    client->setHomingDetectionVelocity(33554432);
    client->setCommunicationFuntionExtendedSetup(40);
    client->setHomingReturnSpeedLimit(20000);
    client->setHomingMode(34);  // (On Index Pulse +Ve direction)
    client->setTouchProbe(7);

    client->writeOutputs(output);
}

void ppMode(minas_control::MinasClient *client, minas_control::MinasOutput output, minas_control::MinasInput input)
{
    // control model setup (see statusword(6041.h) 3) p.107)
    client->writeOutputs(output);
    while (!(input.statusword & 0x1000))
    { // bit12 (set-point-acknowledge)
        input = client->readInputs();
    }
    output.controlword &= ~0x0010; // clear new-set-point (bit4)
    client->writeOutputs(output);

    printf("target position = %08x\n", output.target_position); // poição absoluta

}