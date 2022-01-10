//
// Created by lq on 2021/12/28.
//

#include <string>
#include <iostream>
#include <vector>
#include "../includes/field_types/Field.h"
#include "../includes/tools/PayloadObjectMap.h"
#include "../includes/tools/PayloadObjectMapFactory.h"

using namespace std;


const CmdManager test_cmd_manager = {3, {
        {"TCRQ", 3, {
                {"TE_SEQ_NO", -1, &SHORT, 0},
                {"CMD", -1, &CHARS_4, "TCRQ"},
                {"REPEAT_COUNT", -1, &SHORT, 0}}},

        {"EIPC", 6, {
                {"TE_SEQ_NO", -1, &SHORT, 0},
                {"CMD", -1, &CHARS_4, "EIPC"},
                {"AFDX_COM_PORT", -1, &SHORT, 0},
                {"SEND_OR_HOLD", 9, &CHARS_4, 0},
                {"S_U", -1, &CHARS_1, 0},
                {"MESSAGE", -1, &VAR_DATA, 0}}},

        {"TEST", 3, {
                {"t1", 3, &CHARS_1, "N"},
                {"t2", -1, &SHORT, 0},
                {"t3", -1, &CHARS_4, "YYYY"}}},
}};

void test1() {
    cout << "\n*** test 1 ***" << endl;
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
    cout << "\n*** test 2: PayloadObjectMap ***" << endl;
    PayloadObjectMap poMap = PayloadObjectMap(CmdMessage(test_cmd_manager.cmds[0]));
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

void test3() {
    cout << "\n*** test 3: CmdMessage ***" << endl;

    Cmd cmd = test_cmd_manager.cmds[1];
    CmdMessage cmdMessage = CmdMessage(cmd);
    cout << (cmdMessage.getCmdPayloadMinSize() == 14) << endl;
    cout << (cmdMessage.isHasUnknownFieldSize()) << endl;
    cout << (cmdMessage.getCmd().fields[2].start_pos == 6) << endl;
    cout << (cmdMessage.getCmd().fields[3].start_pos == 9) << endl;
    cout << (cmdMessage.getCmd().fields[5].start_pos == 14) << endl;

    vector<uint8_t> p_no_var{'\0', '\5', 'E', 'I', 'P', 'C', '\0', '\7', 'S', 'E', 'N', 'D', '\0', 'U'};
    vector<uint8_t> p_short{'\0', '\5', 'E', 'I', 'P', 'C', '\0', '\7', 'S', 'E', 'N', 'D', 'U'};
    vector<uint8_t> p_var{'\0', '\5', 'E', 'I', 'P', 'C', '\0', '\7', 'S', 'E', 'N', 'D', '\0', 'U', 'a', 'b', 'c'};
    cout << cmdMessage.checkRequires(p_no_var) << endl;
    cout << !cmdMessage.checkRequires(p_short) << endl;
    cout << cmdMessage.checkRequires(p_no_var) << endl;
}

void test4() {
    cout << "\n*** test 4: PayloadObjectMapFactory ***" << endl;

    vector<PayloadObjectMap *> m_p_s;

    PayloadObjectMapFactory factory(test_cmd_manager);
    cout << "*a. p_eipc_no_var" << endl;
    vector<uint8_t> p_eipc_no_var{'\0', '\5', 'E', 'I', 'P', 'C', '\0', '\7', '\0', 'S', 'E', 'N', 'D', 'U'};
    auto eipc_no_var = factory.createPOMByPayload(p_eipc_no_var);
    m_p_s.push_back(eipc_no_var);
    cout << (nullptr != eipc_no_var) << endl;
    cout << (eipc_no_var->getNowSize() == p_eipc_no_var.size()) << endl;
    cout << (eipc_no_var->getMinSize() == p_eipc_no_var.size()) << endl;
    cout << (eipc_no_var->getField("TE_SEQ_NO", uint16_t(-1)) == 5) << endl;
    cout << (eipc_no_var->getField("S_U", string("T")) == "U") << endl;
    cout << (eipc_no_var->getField("MESSAGE", vector<uint8_t>(1, -1)).empty()) << endl;

    cout << "*b. p_eipc_short" << endl;
    vector<uint8_t> p_eipc_short{'\0', '\5', 'E', 'I', 'P', 'C', '\0', '\7', 'S', 'E', 'N', 'D', 'U'};
    auto eipc_short = factory.createPOMByPayload(p_eipc_short);
    m_p_s.push_back(eipc_short);
    cout << (nullptr == eipc_short) << endl;

    cout << "*c. p_eipc_var" << endl;
    vector<uint8_t> p_eipc_var{'\0', '\5', 'E', 'I', 'P', 'C', '\0', '\7', '\0', 'S', 'E', 'N', 'D', 'U', 'a', 'b', 'c'};
    auto eipc_var = factory.createPOMByPayload(p_eipc_var);
    m_p_s.push_back(eipc_var);
    cout << (nullptr != eipc_var) << endl;
    cout << (eipc_var->getNowSize() == p_eipc_var.size()) << endl;
    cout << (eipc_var->getMinSize() == (p_eipc_var.size() - 3)) << endl;
    cout << (eipc_var->getField("TE_SEQ_NO", uint16_t(-1)) == 5) << endl;
    cout << (eipc_var->getField("S_U", string("T")) == "U") << endl;
    cout << (eipc_var->getField("MESSAGE", vector<uint8_t>()) == vector<uint8_t>{'a', 'b', 'c'}) << endl;

    cout << "*d. p_tcrp_true" << endl;
    vector<uint8_t> p_tcrp_true{'\2', '\1', 'T', 'C', 'R', 'Q', '\0', '\2'};
    auto tcrp_true = factory.createPOMByPayload(p_tcrp_true);
    m_p_s.push_back(tcrp_true);
    cout << (nullptr != tcrp_true) << endl;
    cout << (tcrp_true->getMinSize() == p_tcrp_true.size()) << endl;
    cout << (tcrp_true->getField("REPEAT_COUNT", uint16_t(-1)) == 2) << endl;

    cout << "*e. p_test_true" << endl;
    vector<uint8_t> p_test_true{'\0', '\0', '\0', 'N', '\0', '\0', 'Y', 'Y', 'Y', 'Y'};
    auto test_ture = factory.createPOMByPayload(p_test_true);
    m_p_s.push_back(test_ture);
    cout << (nullptr != test_ture) << endl;
    cout << (test_ture->getField("t3", string("NNNN")) == "YYYY") << endl;

    cout << "*f. p_test_wrong" << endl;
    vector<uint8_t> p_test_wrong{'\0', '\0', '\0', 'N', '\0', '\0', 'Y', 'Y', 'Y', 'y'};
    auto test_wrong = factory.createPOMByPayload(p_test_wrong);
    m_p_s.push_back(test_wrong);
    cout << (nullptr == test_wrong) << endl;

    for (const auto& p : m_p_s) delete p;
}

int main() {
    test1();
    test2();
    test3();
    test4();
    return 0;
}
