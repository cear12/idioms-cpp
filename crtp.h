#ifndef CRTP_H
#define CRTP_H


#include <QDebug>
#include <QString>

namespace crtp
{
    template< typename Derived >
    class Base
    {
        public:
            void do_something()
            {
                qDebug() << "Base::do_something";
                static_cast< Derived* >( this )->do_something_impl();
            }

      private:
        void do_something_impl()
        {
            qDebug() << "Base::do_something_impl";
        }
    };

    class Foo : public Base< Foo >
    {
    public:
        void do_something_impl()
        {
            qDebug() << "Foo::do_something_impl";
        }
    };

    class Bar : public Base< Bar >
    { };

    void test()
    {
        Base< Foo >().do_something();
    }
}


#endif // CRTP_H
