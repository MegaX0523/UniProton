#include "rpmsg_protocol.h"

// ����ṩ�ĺ���
int rcv_data_from_linux(void *rcv_data, int *data_len);
int send_data_to_linux(void *send_data, int data_len);

// ���մ�������Linux����Ϣ
void process_linux_messages() {
    rpmsg_packet pkt;
    int pkt_size = sizeof(rpmsg_packet);
    int actual_len = pkt_size;
    
    if(rcv_data_from_linux(&pkt, &actual_len) == 0) {
        if(actual_len < sizeof(pkt.msg_type)) return; // ���ݲ�����
        
        switch(pkt.msg_type) {
            case MSG_COMMAND:
                if(actual_len >= sizeof(pkt.msg_type) + sizeof(pkt.payload.command)) {
                    handle_command(pkt.payload.command);
                }
                break;
                
            case MSG_SET_PARAM:
                if(actual_len >= sizeof(pkt.msg_type) + sizeof(ParamPayload)) {
                    update_parameter(pkt.payload.param.param_id, 
                                    pkt.payload.param.param_value);
                }
                break;
        }
    }
}

// ���ʹ��������鵽Linux (ʵʱ��->Linux)
uint8_t send_sensor_array(const SensorArray array) {
    rpmsg_packet pkt = {
        .msg_type = MSG_SENSOR_ARRAY
    };
    memcpy(pkt.payload.array, array, sizeof(SensorArray));
    
    return send_data_to_linux(&pkt, sizeof(pkt.msg_type) + sizeof(SensorArray)) >= 0;
}

// ʾ���������
void handle_command(uint8_t cmd) {
    switch(cmd) {
        case START_EXCITATION:
            start_excitation_system();
            break;
        case STOP_EXCITATION:
            stop_excitation_system();
            break;
        // ... ���������
    }
}

// ʾ���������º���
void update_parameter(uint32_t param_id, double value) {
    switch(param_id) {
        case PARAM_STEP_SIZE:
            set_excitation_freq(value);
            break;
        case PARAM_FREQUENCY:
            set_excitation_amplitude(value);
            break;
        // ... ������������
    }
}