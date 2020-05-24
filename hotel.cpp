#include<iostream>
#include<fstream>
#include <stdlib.h>
#include<cstring>
#include<ctime>
#define file "Menu.txt"
#define databasefile "Database.txt"
using namespace std;

struct item
{
    int sr_no,rate,quantity,prize;
    string item_name;
    item *next;
};

class customer
{
    int table_no;
    string name;
    long long int contact_no;
    item *header;
    string line;
    int total_prize;
    void cancle(item*cn);
    void Insert(string,int);
    void insert_order(int,int,string);
public:
    void take_info();
    void intro();
    void take_order();
    void cancle_item();
    void cancle_order();
    void display_menu();
    void display_bill();
    void exit();
    customer()
    {
        table_no=0;
        total_prize=0;
        header=NULL;
    }
};

void customer::take_info()
{
    cout<<"\nEnter Your Name : ";
    cin>>name;
    cout<<"\nEnter Table no(0 for parcel) : ";
    cin>>table_no;
    cout<<"\nEnter Contact Number : ";
    cin>>contact_no;
}

void customer::cancle(item*cn)
{
    if(cn==NULL)
        return;
    else
    {
        cancle(cn->next);
        delete cn;

    }
}

void customer::Insert(string s,int n)
{
    for(int i=0;i<n;i++)
    cout<<s;
}

void customer::intro()
{
    //system("CLS");
    Insert("-",18);
    cout<<"MAHARAJA HOTEL";
    Insert("-",18);
    cout<<endl;
}

void customer::insert_order(int item_no,int quantity, string line)
{
    item *nn=new item;
    nn->rate=0;
    int j=1;
    for(int i=strlen(&line[0])-4;line[i]!=' ';i--)
    //or
    //for(int i=line.length()-3;line[i]!=' ';i--)
    //for some compiler put -3
    {
        nn->rate += (line[i] - '0')*j;
        j *= 10;
    }
    int i=4;
    nn->item_name="";
    while(line[i]!=' '||line[i+1]!=' ')
    {
        nn->item_name += line[i];
        i++;
    }
    nn->sr_no=item_no;
    nn->quantity=quantity;
    nn->prize = nn->rate * nn->quantity;
    nn->next=NULL;
    if(header==NULL)
        header=nn;
    else if(header->next==NULL)
    {
        if(header->sr_no>nn->sr_no)
        {
            nn->next=header;
            header=nn;
        }
        else if(header->sr_no==nn->sr_no)
        {
            header->quantity += quantity;
            header->prize = header->rate*header->quantity;
            delete nn;
        }
        else
            header->next=nn;
    }
    else if(header->sr_no>=nn->sr_no)
    {
        if(header->sr_no==nn->sr_no)
        {
            header->quantity += quantity;
            header->prize = header->rate*header->quantity;
            delete nn;
        }
        else
        {
            nn->next=header;
            header=nn;
        }
    }
    else
    {
        item *cn;
        cn=header;
        while(cn->next!=NULL)
        {
            if(cn->next->sr_no == nn->sr_no)
            {
                cn=cn->next;
                break;
            }
            else if(cn->next->sr_no > nn->sr_no)
            {
                break;
            }
            cn=cn->next;
        }
        if(cn->sr_no == nn->sr_no)
        {
            cn->quantity += quantity;
            cn->prize = cn->rate*cn->quantity;
            delete nn;
        }
        else
        {
            nn->next=cn->next;
            cn->next=nn;
        }
    }
}

void customer::display_menu()
{
    //int max1=0;
    for(int i=0;i<12;i++)
    cout<<"-";
    cout<<"Food Menu";
    for(int i=0;i<17;i++)
    cout<<"-";
    cout<<endl;
    ifstream menu;
    menu.open(file,ios::out);
    while(menu)
    {
        getline(menu,line);
        //if(line.size() > max1)
        //    max1=line.size();
        if(line[0]=='#')
            cout<<"\t";
        cout<<line<<endl;
        if(menu.eof()!=0)
            break;
    }
    menu.close();
    for(int i=0;i<38;i++)
    cout<<"-";
    cout<<endl;
    //cout<<max1;
}

void customer::take_order()
{
    int ch,quantity;
    char _continue;
    do
    {
        ifstream menu;
        menu.open(file,ios::out);
        bool flag=0;
        ch=0;
        cout<<"\nEnter Item Number : ";
        cin>>ch;
        do
        {
            int item_no=0;
            getline(menu,line);
            item_no += (line[0]-48)*100;
            item_no += (line[1]-48)*10;
            item_no += line[2]-48;
            if(ch==item_no)
            {
                flag=1;
                cout<<endl<<line;
                cout<<"\nEnter Quantity : ";
                cin>>quantity;
                insert_order(item_no,quantity,line);
                cout<<"\nWould you like to order anything else? (y/n) : ";
                cin>>_continue;
                break;
            }
        }while(menu);
        if(!flag)
        {
            cout<<"\nPlease enter valid order number..."<<endl;
            _continue='y';
        }
        menu.close();
    }while(_continue!='n');
}

void customer::cancle_item()
{
    if(header!=NULL)
    {
        cout<<"\nYour Order list :-\n";
        item *cn=header;
        while(cn!=NULL)
        {   cout<<cn->sr_no<<"."<<cn->item_name<<" Quantity:"<<cn->quantity<<endl;
            cn=cn->next;
        }
        int ch;
        int quantity=0;
        bool flag=0;
        cout<<"\nEnter Item Number : ";
        cin>>ch;
        cn=header;
        while(cn!=NULL)
        {
            if(cn->sr_no==ch)
            {
                flag=1;
                cout<<"\nEnter Quantity : ";
                cin>>quantity;
                item *temp=header;
                if(header==cn)
                {
                    if(quantity==cn->quantity)
                    {
                        header=header->next;
                        cout<<endl<<temp->item_name<<" cancled..."<<endl;
                        delete temp;
                    }
                    else if(quantity>cn->quantity)
                    {
                        cout<<"\nInvalid Quantity...\n";
                        continue;
                    }
                    else
                    {
                        cn->quantity -= quantity;
                        cn->prize=cn->rate*cn->quantity;
                        cout<<endl<<temp->item_name<<" qunatity "<<quantity<<" cancled..."<<endl;
                    }

                }
                else
                {
                    if(quantity==cn->quantity)
                    {
                        while(temp->next!=cn)
                            temp=temp->next;
                        temp->next=cn->next;
                        cout<<endl<<cn->item_name<<" cancled..."<<endl;
                        delete cn;
                    }
                     else if(quantity>cn->quantity)
                    {
                        cout<<"\nInvalid Quantity...\n";
                        continue;
                    }
                    else
                    {
                        cn->quantity -= quantity;
                        cn->prize=cn->rate*cn->quantity;
                        cout<<endl<<cn->item_name<<" qunatity "<<quantity<<" cancled..."<<endl;
                    }
                }
                break;
            }
            cn=cn->next;
        }
        if(!flag)
            cout<<"\nItem does not exist in your order..."<<endl;
    }
    else
    cout<<"\nYour Order List is Empty...\n";
}

void customer::cancle_order()
{
    if(header!=NULL)
    {
        cancle(header);
        header=NULL;
        cout<<"\nYour Order is Canceled...\n";
    }
    else
        cout<<"\nYour Order List is Empty...\n";
}

 void customer::display_bill()
{
    intro();
    if(header==NULL)
        cout<<"\nYour Order list is empty...\n";
    else
    {
        Insert(" ",18);
        cout<<"Pimple Gurav\n";
        Insert(" ",18);
        cout<<"Pune-411061\n";
        cout<<"\t    GST.NO :- 27AHXPP3379HIZH\n";
        Insert("-",23);
        cout<<"BILL";
        Insert("-",23);
        cout<<"\nTABLE : ";
        if(table_no<10)
            cout<<"0";
        cout<<table_no;
        cout<<"\nDate : ";
        time_t now = time(0);
        tm *time = localtime(&now);
        cout<<time->tm_mday<<"/"<<1+time->tm_mon<<"/"
            <<1900+time->tm_year<<" (";
        srting wday = ["Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"]
        cout<<wday[time->tm_wday-1]
        cout<<")"<<"\nTime : "<<time->tm_hour
            <<"Hr "<<time->tm_min<<"Min "
            <<time->tm_sec<<"Sec"<<endl;
        Insert("-",50);
        cout<<"\nDESCRIPTION";
        Insert(" ",10);
        cout<<"QUANTITY";
        Insert(" ",5);
        cout<<"RATE";
        Insert(" ",5);
        cout<<"AMOUNT\n";
        Insert("-",50);
        cout<<endl;
        item *cn=header;
        total_prize=0;
        while(cn!=NULL)
        {
            int a;
            total_prize += cn->prize;
            cout<<"*";
            if(cn->item_name.size()>24)
            {
                string s = cn->item_name.substr(0,24);
                int i=23;
                while(s[i]!= ' ')
                {
                    i--;
                }
                a=i;
                for(int j=0;j<i;j++)
                cout<<s[j];
                Insert(" ",24-a);
            }
            else
            {
                cout<<cn->item_name;
                Insert(" ",24-cn->item_name.size());
            }
            cout<<cn->quantity;
            Insert(" ",8);
            cout<<cn->rate;
            if(cn->rate>99)
                Insert(" ",8);
            else
                Insert(" ",9);
            cout<<cn->prize<<endl;
            if(cn->item_name.size()>24)
            {
                cout<<"   ";
                for(int i=a+1;i<cn->item_name.size();i++)
                    cout<<cn->item_name[i];
                cout<<endl;
            }
            cn=cn->next;
        }
        cout<<"\n\tTotal price : "<<total_prize<<endl;
        Insert("-",50);
        cout<<endl;

    }
}

void customer::exit()
{
    if(header!=NULL)
    {
        ofstream data;
        data.open(databasefile, ios::in | ios::app);
        data<<"Name : "<<name<<"\nTabel No : "<<table_no<<"\nContact : "<<contact_no
            <<"\nDate and Time : ";
        time_t now = time(0);
        tm *time = localtime(&now);
        data<<time->tm_mday<<"/"<<1+time->tm_mon<<"/"
            <<1900+time->tm_year<<" (";
        switch(time->tm_wday)
        {
        case 1:
            data<<"Monday";
            break;
        case 2:
            data<<"Tuesday";
            break;
        case 3:
            data<<"Wednesday";
            break;
        case 4:
            data<<"Thursday";
            break;
        case 5:
            data<<"Friday";
            break;
        case 6:
            data<<"Saturday";
            break;
        case 7:
            data<<"Sunday";
        }
        data<<")  "<<time->tm_hour
            <<"Hr "<<time->tm_min<<"Min "
            <<time->tm_sec<<"Sec";
        data<<"\nOrder :- ";
        item *cn=header;
        while(cn!=NULL)
        {
            data<<"\nDescription : "<<cn->item_name<<"/Quantity : "<<cn->quantity
                <<"/Rate : "<<cn->rate<<"/Prize : "<<cn->prize;
            cn=cn->next;
        }
        data<<"\nTotal Prize : "<<total_prize;
        data<<endl<<endl;
        data.close();
        cancle(header);
    }
}

int main()
{
    int ch;
    customer c;
    c.intro();
    c.take_info();
    do
    {
        cout<<"\n1.Display Menu";
        cout<<"\n2.Take Order";
        cout<<"\n3.Cancel Item";
        cout<<"\n4.Cancel Order";
        cout<<"\n5.Display Bill";
        cout<<"\n6.Exit";
        cout<<"\n\nEnter Your Choice : ";
        cin>>ch;
        switch(ch)
        {
        case 1:
            c.intro();
            c.display_menu();
            break;
        case 2:
            c.intro();
            c.take_order();
            break;
        case 3:
            c.intro();
            c.cancle_item();
            break;
        case 4:
            c.intro();
            c.cancle_order();
            break;
        case 5:
            //system("CLS");
            c.display_bill();
            break;
        case 6:
            c.exit();
            break;
        default:
            cout<<"\nPlease Enter Valid Choice...\n";
        }
    }while(ch!=6);
    return 0;
}
