
#include <sstream>
#include "message.h"

std::string Message::serialize() {
    char div_char = ':';
    std::ostringstream oss;

    oss << msg_type;
    oss << div_char;

    oss << view_num;
    oss << div_char;

    oss << client_id;
    oss << div_char;

    oss << client_seq_num;
    oss << div_char;

    oss << seq_num;
    oss << div_char;

    oss << value;
    oss << div_char;

    oss << sender.port;
    oss << div_char;
    oss << sender.host;
    oss << div_char;

    oss << receivers.size();
    oss << div_char;
    for (auto& r : receivers) {
        oss << r.port;
        oss << div_char;
        oss << r.host;
        oss << div_char;
    }

    oss << acceptor_state.size();
    oss << div_char;
    for (auto& v : acceptor_state) {
        oss << v.view_num;
        oss << div_char;
        oss << v.value;
        oss << div_char;
    }
    return oss.str();
}

void Message::deserialize(std::string in) {

    std::vector<std::string> array;
    size_t pos = 0, found;
    while ((found = in.find_first_of(':', pos)) != std::string::npos) {
        array.push_back(in.substr(pos, found - pos));
        pos = found + 1;
    }
    array.push_back(in.substr(pos));

    int index = 0;
    int msg_int = stoi(array.at(index++));
    msg_type = static_cast<MessageType>(msg_int);
    view_num = stoi(array.at(index++));
    client_id = array.at(index++);
    client_seq_num = stoi(array.at(index++));
    seq_num = stoi(array.at(index++));
    value = array.at(index++);
    sender.port = stoi(array.at(index++));
    sender.host = array.at(index++);
    int num_recv = stoi(array.at(index++));
    // COUT << "Num recv: " << num_recv << endl;
    for (int k = 0; k < num_recv; k++) {
        // COUT << "at: " << array.at(start + i) << " " << array.at(start + 1 + i) << endl;
        node r;
        r.port = stoi(array.at(index++));
        r.host = array.at(index++);
        receivers.push_back(r);
    }
    num_recv = stoi(array.at(index++));
    for (int k = 0; k < num_recv; k++) {
        view_val v;
        v.view_num = stoi(array.at(index++));
        v.value = array.at(index++);
        acceptor_state.push_back(v);
    }
    // COUT << "Num recv: " << num_recv << endl;
}

