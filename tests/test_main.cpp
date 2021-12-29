//
// Created by lq on 2021/12/28.
//

#include <string>
#include <iostream>
#include <vector>
#include "../field_types/Field.h"
#include "../tools/PayloadObjectMap.h"

using namespace std;


void test1() {
    cout << "*** test 1 ***" << endl;
    string s1 = "1234";
    vector<uint8_t> v1 = {'1', '2', '3', '4'};
    auto v = stringToPayload(s1, 4);
    bool s1_v1 = std::equal(v.begin(), v.end(), v1.begin());
    cout << s1_v1 << endl;
    string s2 = payloadToChars(v, 4);
    cout << (s1 == s2) << endl;

    short short1 = 257;
    auto v2 = intToPayload(short1, 2);
    auto short2 = payloadToUnsignedInt(v2, 2, (short)-1);
    cout << (short1 == short2) << endl;
}

void test2() {
    cout << "*** test 2: PayloadObjectMap ***" << endl;
    PayloadObjectMap poMap = PayloadObjectMap(cmd_manager.cmds[0]);
    vector<uint8_t> payload_t = {'\0', '\1', 'T', 'C', 'R', 'Q', '\0', '\2'};
    poMap.loadPayload(payload_t);
    auto cmd_s = poMap.getField("CMD", string("ERR0"));
    cout << (cmd_s == string("TCRQ")) << endl;
    auto field = poMap.getField("TE_SEQ_NO");
    auto seq_no_any = field->getValue();
    // unsigned short 不能转换为 short，建议写作 uint16_t
    auto seq_no = any_cast<uint16_t>(seq_no_any);
    cout << (seq_no == 1) << endl;
    delete field;
    poMap.setField("CMD", Field::createFieldFromValue(&CHARS_4, "EEEE"), true);
    auto cmd_s_1 = poMap.getField("CMD", string("ERR0"));
    cout << (cmd_s_1 == string("EEEE")) << endl;
}

int main() {
    test1();
    test2();
    return 0;
}
