#ifndef KEY_H
#define KEY_H

#include <QThread>
#include  "pthread.h"
#include <linux/input.h>
#include <fcntl.h>
#include "stdio.h"

class Key : public QThread
{
Q_OBJECT
private:
    Key();
    Key(const Key &s)=delete;
    Key& operator =(const Key& s)=delete;
    static Key *Key_Global;
public:
    static Key* Get_Key_Global()
    {
        return Key_Global;
    }
    static void Key_Start_server()
    {
        Key_Global=new Key();
    }
private:
    int input_fd;
#define INPUT_DEVICE "/dev/input/event0"
protected:
    void run();

signals:
    void push_key(unsigned int key_code,unsigned char key_value);


};



#endif // KEY_H
