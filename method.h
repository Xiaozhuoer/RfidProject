#include <QSerialPort>

#ifndef METHOD_H
#define METHOD_H


class method
{
public:
    method();

    void receiveInfo();

    void traCareNumInfo();

    void requestCare();

    void requestAllCare();

    void openAntenna();

private:
    QSerialPort* serialPort;
};

#endif // METHOD_H
