#include <iostream>
#include "Person.pb.h"


class MyTest
{
public: 
    void test()
    {
        //���л�
        Person p;
        p.set_id(10);

        //���붯̬�����ڴ�
        p.add_name("zhangsan");
        p.add_name("�Ϻ�");
        p.add_name("����");


        p.set_sex("man");
        p.set_age(32);
        auto addr = p.mutable_addr();
        addr->set_addr("����");
        addr->set_num(10010);

        

        std::string output;
        p.SerializeToString(&output);

        //�����л�
        Person pp;
        pp.ParseFromString(output);
        std::cout << pp.id() << std::endl;

        int size = pp.name_size();
        for(int i =0; i<size; i++)
        {
            std::cout << pp.name(i) << std::endl;
        }

        std::cout << pp.sex() << std::endl;
        std::cout << pp.age() << std::endl;

        std::cout << pp.addr().addr() << std::endl;
        std::cout<< pp.addr().num() << std::endl;

    }
};

int main(void)
{
   MyTest t;
   t.test();

   return 0;
}