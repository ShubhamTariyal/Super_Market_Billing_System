//****************************************************************
// HEADER FILE USED IN PROJECT
//****************************************************************

#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<limits>

using namespace std;

//****************************************************************
// macro for clear screen
//****************************************************************

#if defined(_WIN32) && !defined(__linux__)
    #define clr_scr() std::clr_scr();
#elif defined(__linux__) && !defined(_WIN32)
    #define clr_scr() [](){return std::system ("clear");}()
#else
    #error OS should be Windows or Linux
#endif

//****************************************************************
// function to remove a line after showing a message from
// input stream
//****************************************************************

bool isempty(fstream& f)
{
    return f.peek() == ifstream::traits_type::eof();
}

//****************************************************************
// function to remove a line after showing a message from
// input stream
//****************************************************************

void rmv_line(string msg = "")
{
    if(msg != "")
    {
        cout << msg;
        cout.flush();
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//****************************************************************
// function to move the forward pos of cursor by some lines and
// spaces
//****************************************************************

void give_space(int i, int j)
{
    while(j--)
    {
        printf("\n");
    }
    while(i--)
        printf(" ");
}

//****************************************************************
// CLASS USED IN PROJECT
//****************************************************************

class Product
{
    int pID;
    string name;
    float price, tax, discount;//stock/qty

public:

    int getpID();
    float getprice();
    string getname();
    int getdiscount();

    void get_detail();
    void show_detail();

    bool read(std::istream& f);
    void write(std::ostream& f);

};

void Product::get_detail()
{
    cout << "\nPlease Enter The Product ID of The Product ";
    cin >> pID;
    cout << "\n\nPlease Enter The Name of The Product ";
    rmv_line();
    getline(cin, name);
    cout << "\nPlease Enter The Price of The Product ";
    cin >> price;
    cout << "\nPlease Enter The Discount (%) ";
    cin >> discount;
}

void Product::show_detail()
{
    cout << "\nThe Product ID of The Product : " << pID;
    cout << "\nThe Name of The Product : ";
    cout << name;
    cout << "\nThe Price of The Product : " << price;
    cout << "\nDiscount : " << discount;
}

void Product::write(std::ostream& f)
{
    size_t size;

    // we need to store the data from the string along with the size
    // because to restore it we need to temporarily read it somewhere
    // before storing it in the std::string (istream::read() doesn't
    // read directly to std::string)

    f.write( (char*)&pID, sizeof(int) );
    f.write( (char*)&price, sizeof(float) );
    f.write( (char*)&discount, sizeof(float) );
    //f.write( (char*)&tax, sizeof(float) );

    size = name.size();
    f.write( (char*)&size, sizeof(size_t) );
    f.write( (char*)name.c_str(), size );

}

bool Product::read(std::istream& f)
{
    size_t size;
    char *data;

        // when we read the string data we need somewhere to store it
        // because we std::string isn't a primitive type.  So we read
        // the size, allocate an array, read the data into the array,
        // load the std::string, and delete the array

    f.read( (char*)&pID, sizeof(int) );
    if (!f)
        return false;
    f.read( (char*)&price, sizeof(float) );
    if (!f)
        return false;
    f.read( (char*)&discount, sizeof(float) );
    //if (!f)
    //    return false;
    //f.read( (char*)&tax, sizeof(float) );

    if (!f)
        return false;
    f.read( (char*)&size, sizeof(size) );
    //name.resize(size);
    //f.read((char *)&name[0], size);
    data = new char[size+1];
    if (!f)
        return false;
    f.read( data, size );
    data[size]='\0';
    name = data;
    delete data;

    if (!f)
        return false;
    return true;

}

int Product::getpID()
{
    return pID;
}

float Product::getprice()
{
    return price;
}

string Product::getname()
{
    return name;
}

int Product::getdiscount()
{
    return discount;
}

//****************************************************************
// global declaration for file stream object, Product object
//****************************************************************

fstream fp;
Product pr;

//****************************************************************
// function to write in file
//****************************************************************

bool write_Product()
{
    char ch;
    fp.open("Shop.dat",ios::out|ios::app);
    if(!fp.is_open())
    {
        cout << "Cannot open file";
        rmv_line("\nPress Enter to continue...");
        return false;
    }
    do
    {
        pr.get_detail();
        pr.write(fp);
        //fp.write((char *)&pr, sizeof(Product));
        cout << "\n\nThe Product Has Been Created ";
        cout << "\n\nWant to create more Products?(y/n) \n";
        cin >> ch;
    }while(ch == 'y' || ch == 'Y');
    fp.close();
    rmv_line();
    return true;
}

//****************************************************************
// function to read all records from file
//****************************************************************

bool display_all()
{
    clr_scr();
    fp.open("Shop.dat",ios::in);
    if(!fp.is_open())
    {
        cout << "\nFile does not exist!\n";
        rmv_line("\nPress Enter to continue...");
        return false;
    }
    cout << "\n\n\n\t\tDISPLAY ALL RECORD !!!\n\n";
    //while(fp.read((char*)&pr, sizeof(Product)))
    while(pr.read(fp))
    {
        pr.show_detail();
        cout << "\n\n====================================\n";
        rmv_line();
    }
    fp.close();
    rmv_line("\nPress Enter to continue...");
    return true;
}

//****************************************************************
// function to read specific record from file
//****************************************************************

bool display_Product()
{
    fp.open("Shop.dat",ios::in);
    if(!fp.is_open())
    {
        cout << "\nFile does not exist!\n";
        rmv_line("\nPress Enter to continue...");
        return false;
    }
    int ID;
    char ch;

    do
    {
        clr_scr();
        fp.clear();

        bool flag = false;

        cout << "\nPlease Enter The Product ID: ";
        cin >> ID;

        fp.seekg(0, ios::beg);
        //while(fp.read((char*)&pr, sizeof(Product)))
        while(pr.read(fp))
        {
            if(pr.getpID() == ID)
            {
                pr.show_detail();
                flag = true;
                break;
            }
        }
        if(flag == 0)
        {
            cout << "\n\nRecord doesn't exist";
        }
        cout << "\n\nDisplay  more records?(y/n): ";
        cin >> ch;
    }while(ch == 'y' || ch == 'Y');
    fp.close();
    rmv_line();
    return true;
}

//****************************************************************
// function to modify record of file
//****************************************************************

bool modify_Product()
{
    char ch;
    fstream temp_fp;
    do
    {
        clr_scr();
        fp.open("Shop.dat",ios::in|ios::out);
        temp_fp.open("Temp.dat",ios::out);
        if(!fp.is_open() || !temp_fp.is_open())
        {
            cout << "\nFile does not exist!\n";
            rmv_line("\nPress Enter to continue...");
            return false;
        }
        fp.seekg(0, ios::beg);
        int ID;
        bool found = false;
        cout << "\n\n\n\tModify Record";
        cout << "\n\nPlease Enter The Product ID: ";
        cin >> ID;
        //while(fp.read((char*)&pr, sizeof(Product)))
        while(pr.read(fp))
        {
            if(pr.getpID() != ID)
            {
                pr.write(temp_fp);
                //temp_fp.write((char*)&pr, sizeof(Product));
            }
            else
            {
                found = true;
                pr.show_detail();
                cout << "\nPlease Enter The New Details of Product" << endl;
                pr.get_detail();
                pr.write(temp_fp);
                cout << "\n\n\t Record Updated";
            }
        }
        if(!found)
        {
            cout << "\n\nProduct not found";
        }
        temp_fp.close();
        fp.close();
        remove("Shop.dat");
        rename("Temp.dat","Shop.dat");
        cout << "\n\nModify  more records?(y/n) \n";
        cin >> ch;
    }while(ch == 'y' || ch == 'Y');
    rmv_line();
    return true;
}

//****************************************************************
// function to delete record of file
//****************************************************************

bool delete_Product()
{
    char ch;
    fstream temp_fp;
    do
    {
        clr_scr();
        fp.open("Shop.dat",ios::in|ios::out);
        temp_fp.open("Temp.dat",ios::out);
        if(!fp.is_open() || !temp_fp.is_open())
        {
            cout << "\nFile does not exist!\n";
            rmv_line("\nPress Enter to continue...");
            return false;
        }
        fp.seekg(0, ios::beg);
        int ID;
        bool check = false;
        cout << "\n\n\n\tDelete Record";
        cout << "\n\nPlease Enter The Product ID: ";
        cin >> ID;
        //while(fp.read((char*)&pr, sizeof(Product)))
        while(pr.read(fp))
        {
            if(pr.getpID() != ID)
            {
                pr.write(temp_fp);
                //temp_fp.write((char*)&pr, sizeof(Product));
            }
            else
            {
                check = true;
            }
        }
        if(check)
        {
            cout << "\n\n\tRecord Deleted...";
        }
        else
        {
            cout << "\n\nProduct not  found";
        }
        temp_fp.close();
        fp.close();
        remove("Shop.dat");
        rename("Temp.dat","Shop.dat");
        cout << "\n\nDelete  more records?(y/n) \n";
        cin >> ch;
    }while(ch == 'y' || ch == 'Y');
    return true;
}

//****************************************************************
// function to display all Products menu
//****************************************************************

bool menu()
{
    clr_scr();
    fp.open("Shop.dat",ios::in);
    if(!fp.is_open())
    {
        cout << "\nERROR!!! FILE DOESN'T EXIST OR COULD NOT BE OPENED";\
        cout << "\n\n\nGo To Admin Menu to create File";
        rmv_line("\nPress Enter to continue...");
        return false;
    }
    if(isempty(fp))
    {
        cout << "\nNo Items in Menu";\
        cout << "\n\n\nGo To Admin Menu to Add Items";
        rmv_line("\nPress Enter to continue...");
        return false;
    }
    cout << "\n\n\t\tProduct MENU\n\n";
    cout << "====================================================\n";
    cout << "P.NO.\t\tNAME\t\tPRICE\n";
    cout << "====================================================\n";
    //while(fp.read((char*)&pr, sizeof(Product)))
    while(pr.read(fp))
    {
        cout << pr.getpID() << "\t\t" << pr.getname() << "\t\t" << pr.getprice() << endl;
    }
    fp.close();
    return true;
}

//***************************************************************
// function to generate bill for Products
//***************************************************************

void show_order(int order_ID[], int quantity[], int c)
{
    clr_scr();
    float amt, damt, total=0;
    cout << "\n\n********************************INVOICE************************\n";
    cout << "\nPr ID\tPr Name\tQuantity \tPrice \tAmount \tAmount after discount\n";
    for(int x = 0; x <= c; ++x)
    {
        fp.open("Shop.dat",ios::in);
        //fp.read((char*)&pr, sizeof(Product));
        while(pr.read(fp))//!fp.eof()
        {
            if(pr.getpID() == order_ID[x])
            {
                amt = pr.getprice() * quantity[x];
                damt = amt - (amt * pr.getdiscount()/100);
                cout << "\n" << order_ID[x] << "\t" << pr.getname() << "\t";
                cout << quantity[x] << "\t\t" << pr.getprice() << "\t";
                cout << amt << "\t\t" << damt;
                total += damt;
            }
            //fp.read((char*)&pr, sizeof(Product));
        }
        fp.close();
    }
    cout << "\n\n\t\t\t\t\tTOTAL = " << total;
    rmv_line();
    rmv_line("\nPress Enter to continue...");
}

//***************************************************************
// function to place order
//***************************************************************

bool place_order()
{
    bool status = menu();
    if(!status)
    {
        return false;
    }
    int order_ID[100], quantity[100], c = 0;
    char ch = 'Y';
    cout << "\n============================";
    cout << "\n PLACE YOUR ORDER";
    cout << "\n============================\n";
    do{
        cout << "\n\nEnter The Product ID Of The Product : ";
        cin >> order_ID[c];
        cout << "\nQuantity in number : ";
        cin >> quantity[c];
        ++c;
        cout << "\nDo You Want To Order Another Product ? (y/n)";
        cin >> ch;
    }while((ch == 'y' ||ch == 'Y') && c < 100);
    cout << "\n\nThank You For Placing The Order";
    show_order(order_ID, quantity, c);
    return true;
}

//****************************************************************
// INTRODUCTION FUNCTION
//****************************************************************

void intro()
{
    clr_scr();
    give_space(35,5);
    cout << "SUPER  MARKET";
    give_space(38,3);
    cout << "BILLING";
    give_space(38,3);
    cout << "PROJECT";
    give_space(2,3);
    cout << "MADE BY : SHUBHAM TARIYAL";
    rmv_line();
}

//****************************************************************
// ADMINSTRATOR MENU FUNCTION
//****************************************************************
void admin_menu()
{
    char ch;
    do
    {
        clr_scr();
        cout << "\n\n\n\tADMIN MENU";
        cout << "\n\n\t1.CREATE PRODUCT";
        cout << "\n\n\t2.DISPLAY ALL PRODUCTS";
        cout << "\n\n\t3.QUERY ";
        cout << "\n\n\t4.MODIFY PRODUCT";
        cout << "\n\n\t5.DELETE PRODUCT";
        cout << "\n\n\t6.VIEW PRODUCT MENU";
        cout << "\n\n\t7.BACK TO MAIN MENU";
        cout << "\n\n\tPlease Enter Your Choice (1-7) ";
        cin >> ch;
        rmv_line();
        while(ch < '1' || ch > '7')
        {
            cout << "\n\tEnter number between 1-7 :";
            cin >> ch;
            rmv_line();
        }
        switch(ch)
        {
            case '1': clr_scr();
                      write_Product();
                      break;

            case '2': display_all();
                      break;

            case '3': display_Product();
                      break;

            case '4': modify_Product();
                      break;

            case '5': delete_Product();
                      break;

            case '6': if(menu())
                        {
                            rmv_line("\nPress Enter to continue...");
                        }

            case '7': break;
        }
    }while(ch != '7');
}

void main_menu()
{
    char ch;
    do
    {
        clr_scr();
        cout << "\n\n\n\tMAIN MENU";
        cout << "\n\n\t01. CUSTOMER";
        cout << "\n\n\t02. ADMINISTRATOR";
        cout << "\n\n\t03. EXIT";
        cout << "\n\n\tPlease Select Your Option (1-3) ";
        cin >> ch;
        // to_del rmv_line();
        while(ch < '1' || ch > '3')
        {
            cout << "\n\tEnter number between 1-3 :";
            cin >> ch;
            // to_del rmv_line();
        }
        switch(ch)
        {
            case '1': clr_scr();
                      place_order();
                      break;

            case '2': admin_menu();
                      break;

            case '3': cout << "\n\tTHANK YOU!!!";
                      exit(0);
        }

    }while(ch!='3');
}

//****************************************************************
// THE MAIN FUNCTION OF PROGRAM
//****************************************************************

int main()
{
    intro();
    main_menu();
}

//***************************************************************
// END OF PROJECT
//***************************************************************
