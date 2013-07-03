#include <iostream>
#include <string>
#include "binio/buffer.h"
#include "binio/formats.h"

using namespace std;
using binio::byte_t;

int main()
{
    cout << "Getting buffer" << endl;
    binio::buffer buff;

    string order;
    while(1)
    {
        cout << "> ";
        cin >> order;

        if(order=="push")
        {
            while(cin.peek()==' ')
            {
                unsigned short byte;
                cin >> byte;
                buff.push(byte);
            }

            continue;
        }

        if(order=="clear")
        {
            buff.free();
            continue;
        }

        if(order=="write")
        {
            while(cin.peek()==' ')
            {
                string type;
                cin >> type;

                //Unsigned integers
                if(type=="byte"){unsigned short x; cin >> x; buff << byte_t(x); continue;}
                if(type=="ushort"){unsigned short x; cin >> x; buff << x; continue;}
                if(type=="uint"){unsigned int x; cin >> x; buff << x; continue;}

				//Signed integers
                if(type=="char"){short c; cin >> c; buff << char(c); continue;}
                if(type=="short"){short x; cin >> x; buff << x; continue;}
                if(type=="int"){int x; cin >> x; buff << x; continue;}

                //Floats
                if(type=="float"){float x; cin >> x; buff << x; continue;}
                if(type=="double"){double x; cin >> x; buff << x; continue;}

                //Strings
                if(type=="string"){char* x=new char[2048]; cin >> x; buff << x; delete [] x; continue;}
            }

            continue;
        }

        if(order=="read")
        {
            while(cin.peek()==' ')
            {
                string type;
                cin >> type;

                //Unsigned integers
                if(type=="byte"){byte_t c; buff >> c; cout << type << ": " << int(c) << endl; continue;}
                if(type=="ushort"){unsigned short c; buff >> c; cout << type << ": " << c << endl; continue;}
                if(type=="uint"){unsigned int c; buff >> c; cout << type << ": " << c << endl; continue;}

                //Signed integers
                if(type=="char"){char c; buff >> c; cout << type << ": " << int(c) << endl; continue;}
                if(type=="short"){short x; buff >> x; cout << type << ": " << x << endl; continue;}
                if(type=="int"){int x; buff >> x; cout << type << ": " << x << endl; continue;}

                //Floats
                if(type=="float"){float x; buff >> x; cout << type << ": " << fixed << x << endl; continue;}
                if(type=="double"){double x; buff >> x; cout << type << ": " << fixed << x << endl; continue;}

                //Strings
                if(type=="string"){char* x=new char[2048]; buff >> x; cout << type << ": " << x << endl; delete [] x; continue;}
            }

            continue;
        }

        if(order=="tell")
        {
            cout << "Current pos: " << buff.tell() << endl;
            continue;
        }

        if(order=="seek")
        {
            size_t s;
            cin >> s;
            buff.seek(s);
            continue;
        }

        if(order=="seekrel")
        {
            int s;
            cin >> s;
            buff.seek(s);
            continue;
        }

        if(order=="pushbit")
        {
            while(cin.peek()==' ')
            {
                bool bit;
                cin >> bit;
                buff.push_bit(bit);
            }

            continue;
        }

        if(order=="flushbits")
        {
            buff.flush_bits();

            continue;
        }

        if(order=="show")
        {
            for(size_t i=0; i<buff.size(); i++)
            {
                cout.width(4);
                cout.fill(' ');
                cout << short(buff[i]);
                if((i+1)%20==0){cout << endl;}
            }
            if(buff.size()%20){cout << endl;}

            continue;
        }

        if(order=="showbits")
        {
            for(size_t i=0; i<buff.size()*8; i++)
            {
                cout << buff.get_bit(i);
                if((i+1)%8==0){cout << " ";}
                if((i+1)%71==0){cout << endl;}
            }
            if(buff.size()%71){cout << endl;}

            continue;
        }

        if(order=="getbit")
        {
            size_t i;
            cin>>i;
            cout << buff.get_bit(i);

            cout << endl;
            continue;
        }

        if(order=="setbit")
        {
            size_t i;
            bool val;
            cin>>i;
            cin >> val;
            buff.set_bit(i,val);
            continue;
        }

        if(order=="exit")
        {
            break;
        }
    }

    return 0;
}
