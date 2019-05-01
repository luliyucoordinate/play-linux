#ifndef _MESSAGEHEADER_HPP_
#define _MESSAGEHEADER_HPP_

enum CMD {
    CMD_LOGIN,
    CMD_LOGIN_RESULT,
    CMD_LOGOUT,
    CMD_LOGOUT_RESULT,
    CMD_NEW_USER_JOIN,
    CMD_ERROR
};

struct DataHeader {
    DataHeader(short dl = 0, short cd = CMD_ERROR) : dataLength(dl), cmd(cd) {}
    short dataLength; //data length
    short cmd;
};

struct Login : public DataHeader {
    Login() : DataHeader(sizeof(Login), CMD_LOGIN) {}

    char userName[32];
    char passWord[32];
};

struct LoginResult : public DataHeader {
    LoginResult() : DataHeader(sizeof(Login), CMD_LOGIN_RESULT), res(0) {}

    int res;
};

struct Logout : public DataHeader {
    Logout() : DataHeader(sizeof(Login), CMD_LOGOUT) {}

    char userName[32];
};

struct LogoutResult : public DataHeader {
    LogoutResult() : DataHeader(sizeof(Login), CMD_LOGOUT_RESULT), res(0) {}

    int res;
};

struct NewUserJoin : public DataHeader {
    NewUserJoin() : DataHeader(sizeof(Login), CMD_NEW_USER_JOIN), sock(0) {}

    int sock;
};

#endif