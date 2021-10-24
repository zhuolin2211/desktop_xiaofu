#include "key.h"
#include "stdio.h"
#include <stdlib.h>
#include "unistd.h"
#include "error.h"
#include "iostream"


Key *Key::Key_Global=nullptr;
Key::Key()
{
    input_fd=open(INPUT_DEVICE,O_RDONLY);
    if(input_fd<0)
    {
        perror("key");
         std::cout<< errno<<std::endl;
         std::cout<<"this input device cant open : "<<INPUT_DEVICE<<std::endl;
    }
    std::cout<<"input device is open"<<std::endl;
    this->start();
}
void Key::run()
{
    struct input_event ev[64];
    int i,rd;
    unsigned int type, code,value;
    fd_set rdfs;
    FD_ZERO(&rdfs);
    FD_SET(input_fd, &rdfs);
    std::cout<<"running key process"<<std::endl;
    while(true)
    {
        select(input_fd + 1, &rdfs, NULL, NULL, NULL);
        rd = read(input_fd, ev, sizeof(ev));
        if (rd < (int) sizeof(struct input_event)) {
            perror("key");
            std::cout<<errno<<std::endl;
            std::cout<<"key read file error"<<std::endl;
            continue;
        }
        for (i = 0; i < rd / sizeof(struct input_event); i++) {
                    type = ev[i].type;
                    code = ev[i].code;
                    value=ev[i].value;
                    if(type == EV_KEY)
                    {
                        switch (code)
                        {
                        case KEY_OK:
                        {
                            emit push_key(1,value);
                            break;
                        }
                        case KEY_SELECT:
                        {
                            emit push_key(2,value);
                            break;
                        }
                        case KEY_VOLUMEDOWN:
                        {
                            emit push_key(3,value);
                            break;
                        }
                        case KEY_VOLUMEUP:
                        {
                            emit push_key(4,value);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                        }
                    }
                    else
                    {
                        std::cout<<"unknow event input "<<std::endl;
                    }
        }
    }
}
