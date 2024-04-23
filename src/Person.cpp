#include <iostream>
#include "Person.pb.h"


class MyTest
{
public: 
    void test()
    {
        //序列化
        Person p;
        p.set_id(10);

        //申请动态数组内存
        p.add_name("zhangsan");
        p.add_name("上海");
        p.add_name("河南");


        p.set_sex("man");
        p.set_age(32);
        auto addr = p.mutable_addr();
        addr->set_addr("北京");
        addr->set_num(10010);

        

        std::string output;
        p.SerializeToString(&output);

        //反序列化
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