#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h> 
#include <stdlib.h> 
#include "clui.h"
using namespace std;

// global var
int user_cnt = 1, admin_is = 0, logged_in_user = -1, today = 50;

// structs
struct food 
{
    bool is;
    string name;
    long long price, quantity, sold;
} food_of_day[100];
struct account 
{
    string id, password, name, last_name, phone_num, reshte, plan;
    long long term, balance;
} user[100], admin;

// prototype
void welcom_page();
void login_page();
void home_page();
void profile();
void samaneh();

// util funcs
void hold()
{
     while(true);
}
void default_text()
{
    change_color_rgb(0, 0, 0);
    printf("\033[48;2;193;156;0m");
}
void admin_text()
{
    reset_color();
    change_color_rgb(19, 161, 14);
}
void cls() 
{
    // clear screen
    restore_cursor();
    reset_color();
    //BG Yellow FG Black
    printf("\e[1;1H\e[2J");
    system("Color 60");
    default_text();
}
void a_cls()
{
    // clear screen
    restore_cursor();
    reset_color();
    //BG Yellow FG Black
    system("Color 02");
    printf("\e[1;1H\e[2J");
}
void hide_text() 
{
    change_color(11);
    change_background_color(2);
}
void print_username()
{
    save_cursor();
    int x = get_window_cols(), y =  get_window_rows();
    cursor_to_pos(y, 0);
    cout << "LOGGED IN AS : " << user[logged_in_user].name << ' ' << user[logged_in_user].last_name;
    restore_cursor();
}
void progress_bar(int i, int t) 
{
    printf("[");
    save_cursor();
    for (int j = 0; j < i; j++)
        cout << ' ';
    printf("]");
    restore_cursor();
    delay(400);
    while (i--)
    {
        cout << '#';
        delay(t);
    }
    cout << "]\n";
}
void esc_bar() 
{
    int x = get_window_cols() / 2, y = get_window_rows();
    cursor_to_pos(y - 2, x - 14);
    progress_bar(26, 80);
    cursor_to_pos(y - 2, x - 14);
    printf("Pleas press <ESC> to proceed");
    while (true) 
    {
        int key = nb_getch();
        if(key != 0)
        {
            if (key == 27)
                break;
            else 
                printf("%d", key);
        }
    }
}
int find_user(string q)
{
    int res = 1;
    while (res < user_cnt)
    {
        if (q == user[res].id)
            return res;
        res++;
    }
    return false;
}
long long str_to_num(string a) 
{
    long long res = 0;
    for (auto c : a)
        res = res * 10 + (c - '0');
        return res;
}
string num_to_str(int num)
{
    string res;
    while (num)
    {
        res += (num % 10) + '0';
        num /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}
string file_i(int i) 
{
    return "accounts\\user" + num_to_str(i) + ".txt";
}
string food_i(int i) 
{
    return "foods\\food" + num_to_str(i) + ".txt";
}
string f_code() {
    string res;
    for (int i = 0; i < 8; i++)
        res += rand() % 26 + 'a';
    return res;
}

//file mannagers
void load_user(int i) 
{
    string file_name = file_i(i);
    ifstream my_file (file_name);
    if (my_file.is_open())
    {
        string line;
        int j = 0;
        while (getline(my_file, line))
        {
            switch (j) 
            {
                case 0:
                    user[i].id = line;
                    break;
                case 1:
                    user[i].password = line;
                    break;
                case 2:
                    user[i].name = line;
                    break;
                case 3:
                    user[i].last_name = line;
                    break;
                case 4:
                    user[i].phone_num = line;
                    break;
                case 5:
                    user[i].reshte = line;
                    break;
                case 6:
                    user[i].term = str_to_num(line);
                    break;
                case 7:
                    user[i].balance = str_to_num(line);
                    break;
                case 8:
                    user[i].plan = line;
                    break;
            }
            j++;
        }
    }
    my_file.close();
}
void load_food(int i)
{
    string file_name = food_i(i);
    ifstream my_file (file_name);

        if (my_file.is_open())
    {
        string line;
        int j = 0;
        while (getline(my_file, line))
        {
            switch (j) 
            {
                case 0:
                    food_of_day[i].name = line;
                    break;
                case 1:
                    food_of_day[i].price = str_to_num(line);
                    break;
                case 2:
                    food_of_day[i].quantity = str_to_num(line);
                    break;
                case 3:
                    food_of_day[i].sold = str_to_num(line);
                    break;
            }
            j++;
        }
    }
    my_file.close();
}
void load_admin() 
{
    admin_is = true;
    string file_name = "accounts\\ADMIN.txt";
    ifstream my_file (file_name);
    if (my_file.is_open())
    {
        string line;
        int j = 0;
        while (getline(my_file, line))
        {
            switch (j) 
            {
                case 0:
                    admin.id = line;
                    break;
                case 1:
                    admin.password = line;
                    break;
            }
            j++;
        }
    }
    admin.name = "SYS", admin.last_name = "ADMIN", admin.balance = 1ll << 62;
    my_file.close();
}
void load_files() 
{
    ifstream my_file;
    my_file.open("accounts\\ADMIN.txt");
    if (my_file)
        load_admin();
    my_file.close();
    string file_name;
    for (int i = 0; i < 100; i++)
    {
        file_name = food_i(i);
        my_file.open(file_name);
        if (my_file)
        {
            load_food(i);
            my_file.close();
            food_of_day[i].is = true;
        }
        else
            food_of_day[i].is = false;
    }
    while (true) 
    {
        file_name = file_i(user_cnt);
        my_file.open(file_name);
        if (my_file)
        {
            load_user(user_cnt++);
            my_file.close();
        }
        else
            return;
    }
}
void save_admin()
{
    string file_name = "accounts\\ADMIN.txt";
    ofstream my_file (file_name);

    my_file << admin.id << '\n';
    my_file << admin.password << '\n';
    my_file.close();
}
void save_account(int i) 
{
    string file_name = file_i(i);
    ofstream my_file (file_name);

    my_file << user[i].id << '\n';
    my_file << user[i].password << '\n';
    my_file << user[i].name << '\n';
    my_file << user[i].last_name << '\n';
    my_file << user[i].phone_num << '\n';
    my_file << user[i].reshte << '\n';
    my_file << user[i].term << '\n';
    my_file << user[i].balance << '\n';
    my_file << user[i].plan << '\n';
    my_file.close();
}
void save_food(int i) 
{
    string file_name = food_i(i);
    ofstream my_file (file_name);

    my_file << food_of_day[i].name << '\n';
    my_file << food_of_day[i].price << '\n';
    my_file << food_of_day[i].quantity << '\n';
    my_file << food_of_day[i].sold << '\n';
    my_file.close();
}

// UI
void admin_page()
{
    if (today < 0)
        today = 0;
    if (today > 99)
        today = 99;
    a_cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("_____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                   |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|             day %d                |", today);
    cursor_to_pos(y + 3, x - 18);
    printf("|          A : to Go back           |");
    cursor_to_pos(y + 4, x - 18);
    printf("|        D : to go forward          |");
    cursor_to_pos(y + 5, x - 18);
    printf("|            S : EDIT               |");
    cursor_to_pos(y + 6, x - 18);
    printf("|   W : Enter day between 0 to 99   |");
    cursor_to_pos(y + 7, x - 18);
    printf("|       Q : Quite to main menu      |");
    cursor_to_pos(y + 8, x - 18);
    printf("|___________________________________|");
    cursor_to_pos(y * 4, x);
    while (true) {
        char key = nb_getch();
        if(key != 0)
        {
            cout << key << '\n';
        delay(1000);
        if (key == 'A' || key == 'a')
            today--, admin_page();
        else if (key == 'D' || key == 'd')
            today++, admin_page();
        else if (key == 'W' || key == 'w')
        {
            cursor_to_pos(y * 4, x);
            printf(" ");
            cursor_to_pos(y * 4, x);
            cin >> today;
            admin_page();
        }
        else if (key == 'S' || key == 's')
            break;
        else if (key == 'Q' || key == 'q')
            welcom_page();
        else   
            cursor_to_pos(y * 4, x);
        } 
    }
    a_cls();
    print_username();
    if (food_of_day[today].is)
    {
        cout << "FOOD ALREADY EXISTS\n" << ' ' << food_of_day[today].name << ' ' << food_of_day[today].price << ' ' << food_of_day[today].quantity << ' ' << food_of_day[today].sold << '\n';
        esc_bar();
        admin_page();
    }
    food_of_day[today].is = true;
    cout << "enter foods name : ";
    cin >> food_of_day[today].name;
    cout << "enter its price and quantity : ";
    cin >> food_of_day[today].price >> food_of_day[today].quantity;
    food_of_day[today].quantity *= 2;
    food_of_day[today].sold = 0;
    a_cls;
    cout << "ALL DONE\nDay " << today << "'s food will be " << food_of_day[today].name << '\n';
    save_food(today);
    esc_bar();
    admin_page();
}
void gozaresh() {
        if (today < 0)
        today = 0;
    if (today > 99)
        today = 99;
    cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("_____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                   |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|             day %d                |", today);
    cursor_to_pos(y + 3, x - 18);
    printf("|          A : to Go back           |");
    cursor_to_pos(y + 4, x - 18);
    printf("|        D : to go forward          |");
    cursor_to_pos(y + 5, x - 18);
    printf("|       S : to get GOZARESH         |");
    cursor_to_pos(y + 6, x - 18);
    printf("|   W : Enter day between 0 to 99   |");
    cursor_to_pos(y + 7, x - 18);
    printf("|       Q : Quite to main menu      |");
    cursor_to_pos(y + 8, x - 18);
    printf("|___________________________________|");
    cursor_to_pos(y * 4, x);
    while (true) {
        char key = nb_getch();
        if(key != 0)
        {
            cout << key << '\n';
        delay(1000);
        if (key == 'A' || key == 'a')
            today--, gozaresh();
        else if (key == 'D' || key == 'd')
            today++, gozaresh();
        else if (key == 'W' || key == 'w')
        {
            cursor_to_pos(y * 4, x);
            printf(" ");
            cursor_to_pos(y * 4, x);
            cin >> today;
            gozaresh();
        }
        else if (key == 'S' || key == 's')
            break;
        else if (key == 'Q' || key == 'q')
            home_page();
        else   
            cursor_to_pos(y * 4, x);
        } 
    }
    cls();
    print_username();
    system("color 20");
    cout << "IN DAY " << today << " HAS SOLD " << food_of_day[today].sold << "HALF PORTIONS (ANY FULL PORTION COUNTS AS 2 HALF ONE'S)\n AND THEY HAVE SOLD $" << food_of_day[today].sold * food_of_day[today].price / 2 << '\n';
    esc_bar();
    gozaresh();
}
void reserve()
{
    if (today < 0)
        today = 0;
    if (today > 99)
        today = 99;
    cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("_____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                   |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|             day %d                |", today);
    cursor_to_pos(y + 3, x - 18);
    printf("|          A : to Go back           |");
    cursor_to_pos(y + 4, x - 18);
    printf("|        D : to go forward          |");
    cursor_to_pos(y + 5, x - 18);
    printf("|         S : to reserve            |");
    cursor_to_pos(y + 6, x - 18);
    printf("|   W : Enter day between 0 to 99   |");
    cursor_to_pos(y + 7, x - 18);
    printf("|       Q : Quite to main menu      |");
    cursor_to_pos(y + 8, x - 18);
    printf("|___________________________________|");
    cursor_to_pos(y * 4, x);
    while (true) {
        char key = nb_getch();
        if(key != 0)
        {
            cout << key << '\n';
        delay(1000);
        if (key == 'A' || key == 'a')
            today--, reserve();
        else if (key == 'D' || key == 'd')
            today++, reserve();
        else if (key == 'W' || key == 'w')
        {
            cursor_to_pos(y * 4, x);
            printf(" ");
            cursor_to_pos(y * 4, x);
            cin >> today;
            reserve();
        }
        else if (key == 'S' || key == 's')
            break;
        else if (key == 'Q' || key == 'q')
            home_page();
        else   
            cursor_to_pos(y * 4, x);
        } 
    }
    if (today < 52) {
        cls();
        print_username();
        system("color 40");
        cout << "today is day 50 so you can't resverve for today :((((\n";
        esc_bar();
        reserve();
    }
    if (!food_of_day[today].is) {
        cls();
        print_username();
        system("color 20");
        cout << "404\nFOOD NOT FOUND";
        esc_bar();
        reserve();
    }
    char c = user[logged_in_user].plan[today];
    if (c > '0') {
        cls();
        print_username();
        system("color 20");
        cout << "YOU HAVE ALRADY RESERVED A ";
        if (c > '4')
            cout << "HALF SIZED PORTION ";
        else
            cout << "FULL SIZED PORTION ";
        cout << food_of_day[today].name << '\n';
        esc_bar();
        reserve();
    }
    cls();
    print_username();
    system("color 1F");
    cout << "DAY " << today << "'s food is " << food_of_day[today].name << '\n';
    cout << "HALF SIZED PORTION is $" << food_of_day[today].price / 2;
    cout << "\nFULL SIZED PORTION is $" << food_of_day[today].price << '\n';
    cout << "ENTER 1 FOR HALF PORTION, 2 FOR FULL PORTION AND 3 TO QUITE\nyour account balance is $" << user[logged_in_user].balance << '\n';
    x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y * 4, x);
    while (true) {
        char key = nb_getch();
        if(key != 0)
        {
            cout << key << '\n';
        delay(1000);
        if (key == '1')
        {
            if (user[logged_in_user].balance < food_of_day[today].price / 2)
            {
                cls();
                print_username();
                system("color 20");
                cout << "NOT ENOUGH MONEY!\n";
                esc_bar();
                reserve();
            }
            if (!food_of_day[today].quantity)
            {
                cls();
                print_username();
                system("color 20");
                cout << "NOT FOOD LEFT!\n";
                esc_bar();
                reserve();
            }
            user[logged_in_user].balance -= food_of_day[today].price / 2;
            user[logged_in_user].plan[today] = '1';
            food_of_day[today].sold++;
            food_of_day[today].quantity--;
            save_food(today);
            save_account(logged_in_user);
            cls();
            print_username();
            system("color 20");
            cout << "half porting reserved\n";
            esc_bar();
            reserve();
        }
        else if (key == '2')
        {
            if (user[logged_in_user].balance < food_of_day[today].price)
            {
                cls();
                print_username();
                system("color 20");
                cout << "NOT ENOUGH MONEY!\n";
                esc_bar();
                reserve();
            }
            if (2 > food_of_day[today].quantity)
            {
                cls();
                print_username();
                system("color 20");
                cout << "NOT FULL PORTION LEFT!\n";
                esc_bar();
                reserve();
            }
            user[logged_in_user].balance -= food_of_day[today].price;
            user[logged_in_user].plan[today] = '5';
            food_of_day[today].sold += 2;
            food_of_day[today].quantity -= 2;
            save_food(today);
            save_account(logged_in_user);
            cls();
            print_username();
            system("color 20");
            cout << "FULL porting reserved\n";
            esc_bar();
            reserve();
        }
        else if (key == '3')
            reserve();
        else   
            cursor_to_pos(y * 4, x);
        } 
    }

}
void faramooshi()
{
    if (today < 0)
        today = 0;
    if (today > 99)
        today = 99;
    cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("_____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                   |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|             day %d                |", today);
    cursor_to_pos(y + 3, x - 18);
    printf("|          A : to Go back           |");
    cursor_to_pos(y + 4, x - 18);
    printf("|        D : to go forward          |");
    cursor_to_pos(y + 5, x - 18);
    printf("|         S : to get Code           |");
    cursor_to_pos(y + 6, x - 18);
    printf("|   W : Enter day between 0 to 99   |");
    cursor_to_pos(y + 7, x - 18);
    printf("|       Q : Quite to main menu      |");
    cursor_to_pos(y + 8, x - 18);
    printf("|___________________________________|");
    cursor_to_pos(y * 4, x);
    while (true) {
        char key = nb_getch();
        if(key != 0)
        {
            cout << key << '\n';
        delay(1000);
        if (key == 'A' || key == 'a')
            today--, faramooshi();
        else if (key == 'D' || key == 'd')
            today++, faramooshi();
        else if (key == 'W' || key == 'w')
        {
            cursor_to_pos(y * 4, x);
            printf(" ");
            cursor_to_pos(y * 4, x);
            cin >> today;
            faramooshi();
        }
        else if (key == 'S' || key == 's')
            break;
        else if (key == 'Q' || key == 'q')
            home_page();
        else   
            cursor_to_pos(y * 4, x);
        } 
    }
    if (!food_of_day[today].is) {
        cls();
        print_username();
        system("color 20");
        cout << "404\nFOOD NOT FOUND";
        esc_bar();
        faramooshi();
    }
    char c = user[logged_in_user].plan[today];
    if (c == '0') 
    {
        cls();
        print_username();
        system("color 20");
        cout << "YOU DON'T HAVE A RESERVATION";
        esc_bar();
        faramooshi();
    }
    if (c == '4' || c == '8')
    {
        cls();
        print_username();
        system("color 20");
        cout << "YOU HAVE USED ALL OF YOUR 3 CODES";
        esc_bar();
        faramooshi();
    } 
    if (c <= '4')
    {
        c = ++user[logged_in_user].plan[today];
        cls();
        print_username();
        system("color 20");
        cout << "YOU HAVE " << '4' - c << "CODES LEFT\nYOUR CODE IS : " << f_code();
        esc_bar();
        faramooshi();
    } 
    else
    {
        c = ++user[logged_in_user].plan[today];
        cls();
        print_username();
        system("color 20");
        cout << "YOU HAVE " << '8' - c << "CODES LEFT\nYOUR CODE IS : " << f_code();
        esc_bar();
        faramooshi();
    } 
}
void i_a_b()
{
    //Increase Account Balance
    cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows();
    cursor_to_pos(y / 2 - 2, x - 18);
    cout << "YOUR CURRENT BALANCE IS $" << user[logged_in_user].balance;
    cursor_to_pos(y / 2 - 1, x - 18);
    cout << "How much will you deposit?";
    long long m;
    cursor_to_pos((y / 4) * 3, x - 5);
    printf("$");
    cin >> m;
    user[logged_in_user].balance += m;
    cls();
    print_username();
    system("color 20");
    x = get_window_cols() / 2, y =  get_window_rows();
    cursor_to_pos(y / 2 - 2, x - 18);
    cout << "YOUR NEW ACCOUNT BALANCE IS $" << user[logged_in_user].balance << '\n';
    cursor_to_pos(y - 2, x - 14);
    progress_bar(26, 120);
    cursor_to_pos(y - 2, x - 14);
    printf("Pleas press <ESC> to go back");
    while (true) 
    {
        int key = nb_getch();
        if(key != 0)
        {
            if (key == 27)
                break;
            else 
                printf("%d", key);
        }
    }
    save_account(logged_in_user);
    samaneh();
}
void t_a_b()
{
    //Transfer Account Balance 
    cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows();
    cursor_to_pos(y / 2 - 2, x - 18);
    cout << "YOUR CURRENT BALANCE IS $" << user[logged_in_user].balance;
    cursor_to_pos(y / 2 - 1, x - 18);
    cout << "Enter your trget id and amount";
    long long m, hdf;
    string target_id;
    cursor_to_pos((y / 4) * 3 - 1, x - 6);
    printf("id ");
    cursor_to_pos((y / 4) * 3, x - 5);
    printf("$");
    cursor_to_pos((y / 4) * 3 - 1, x - 3);
    cin >> target_id;
    cursor_to_pos((y / 4) * 3, x - 3);
    cin >> m;
    hdf = find_user(target_id);
    if(!hdf) {
        cls();
        print_username();
        cout << "ID NOT FOUND\n";
        esc_bar();
        t_a_b();
    }
    if (m > user[logged_in_user].balance) {
        cls();
        print_username();
        cout << "NOT ENOUGH MONEY !\n";
        esc_bar();
        t_a_b();
    }
    user[logged_in_user].balance -= m;
    user[hdf].balance += m;
    save_account(logged_in_user);
    save_account(hdf);
    cls();
    print_username();
    system("color 20");
    x = get_window_cols() / 2, y =  get_window_rows();
    cursor_to_pos(y / 2 - 2, x - 18);
    cout << "YOUR NEW ACCOUNT BALANCE IS $" << user[logged_in_user].balance << '\n';
    cursor_to_pos(y / 2 - 1, x - 25);
    cout << user[hdf].name << ' ' << user[hdf].last_name << " ACCOUNT BALANCE IS $" << user[hdf].balance << '\n';
    esc_bar();
    samaneh();
}
void samaneh()
{
    cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("________________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                      |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|           1 : Reserve                |");
    cursor_to_pos(y + 3, x - 18);
    printf("|         2 : Faramooshi :)            |");
    cursor_to_pos(y + 4, x - 18);
    printf("|     3 : Increase Account Balance     |");
    cursor_to_pos(y + 5, x - 18);
    printf("|     4 : Transfer Account Balance     |");
    cursor_to_pos(y + 6, x - 18);
    printf("|         5 : GOZARESHE MALI           |");
    cursor_to_pos(y + 7, x - 18);
    printf("|              6 : Back                |");
    cursor_to_pos(y + 8, x - 18);
    printf("|______________________________________|");
    cursor_to_pos(y * 4, x);
    while (true) {
        char key = nb_getch();
        if(key != 0)
        {
            cout << key << '\n';
        delay(1000);
        if (key == '1')
            reserve();
        else if (key == '2')
            faramooshi();
        else if (key == '3')
            i_a_b();
        else if (key == '4')
            t_a_b();
        else if (key == '5')
            gozaresh();
        else if (key == '6')
            home_page();
        else   
            cursor_to_pos(y * 4, x);
        } 
    }
    
}
void change_password()
{
    string old_ps, new_ps;
    cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("_____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                   |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|      Enter your old password      |");
    cursor_to_pos(y + 3, x - 18);
    printf("|___________________________________|");
    cursor_to_pos(y * 4 - 3, 0);
    cout << "OLD PASSWORD : ";
    hide_text();
    cin >> old_ps;
    default_text();
    while (old_ps != user[logged_in_user].password) 
    {
        cls();
        print_username();
        x = get_window_cols() / 2, y =  get_window_rows() / 4;
        cursor_to_pos(y, x - 18);
        printf("_____________________________________"); 
        cursor_to_pos(y + 1, x - 18);
        printf("|                                   |"); 
        cursor_to_pos(y + 2, x - 18);
        printf("|      Wrong! please try again      |");
        cursor_to_pos(y + 3, x - 18);
        printf("|___________________________________|");
        cursor_to_pos(y * 4 - 3, 0);
        cout << "OLD PASSWORD : ";
        hide_text();
        cin >> old_ps;
        default_text();
    }
    do 
    {
        cls();
        print_username();
        x = get_window_cols() / 2, y =  get_window_rows() / 4;
        cursor_to_pos(y, x - 18);
        printf("_____________________________________"); 
        cursor_to_pos(y + 1, x - 18);
        printf("|                                   |"); 
        cursor_to_pos(y + 2, x - 18);
        printf("|      Enter your new password      |");
        cursor_to_pos(y + 3, x - 18);
        printf("|___________________________________|");
        cursor_to_pos(y * 4 - 3, 0);
        cout << "NEW PASSWORD : ";
        hide_text();
        cin >> old_ps;
        default_text();
        cursor_to_pos(y * 4 - 2, 0);
        cout << "CONFIRM NEW PASSWORD : ";
        hide_text();
        cin >> new_ps;
        default_text();
        if (new_ps != old_ps)
        {
            cls();
            print_username();
            x = get_window_cols() / 2, y =  get_window_rows() / 4;
            cursor_to_pos(y, x - 18);
            printf("_____________________________________"); 
            cursor_to_pos(y + 1, x - 18);
            printf("|                                   |"); 
            cursor_to_pos(y + 2, x - 18);
            printf("|      Wrong! please try again      |");
            cursor_to_pos(y + 3, x - 18);
            printf("|___________________________________|");
            x = get_window_cols() / 2, y = get_window_rows();
            cursor_to_pos(y - 2, x - 14);
            progress_bar(26, 120);
        }
    }
    while (new_ps != old_ps);
    user[logged_in_user].password = new_ps;
    cls();
    print_username();
    system("color 20");
    x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("_____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                   |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|          PASSWORD CHANGED         |");
    cursor_to_pos(y + 3, x - 18);
    printf("|___________________________________|");
    esc_bar();
    save_account(logged_in_user);
    profile();
}
void personal_details() 
{
    cls();
    system("color 20");
    print_username();
    cout << "ID : " << user[logged_in_user].id << '\n';
    cout << "FIRST NAME : " << user[logged_in_user].name << '\n';
    cout << "LAST NAME : " << user[logged_in_user].last_name << '\n';
    cout << "PHONE NUMBER : " << user[logged_in_user].phone_num << '\n';
    cout << "ACCOUNT BALANCE : $" << user[logged_in_user].balance << "\n";
    cout << "TERM : " << user[logged_in_user].term << '\n';
    cout << "RESHTE : " << user[logged_in_user].reshte << '\n';
    esc_bar();
    profile();
}
void profile() 
{
    cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("_____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                   |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|       1 : personal details        |");
    cursor_to_pos(y + 3, x - 18);
    printf("|        2 : change password        |");
    cursor_to_pos(y + 4, x - 18);
    printf("|            3 : back               |");
    cursor_to_pos(y + 5, x - 18);
    printf("|___________________________________|");
    cursor_to_pos(y * 4, x);
    while (true) {
        char key = nb_getch();
        if(key != 0)
        {
            cout << key << '\n';
        delay(1000);
        if (key == '1')
            personal_details();
        else if (key == '2')
            change_password();
        else if (key == '3')
            home_page();
        else   
            cursor_to_pos(y * 4, x);
        } 
    }
}
void home_page() 
{
    cls();
    print_username();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                  |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|           1 : profile            |");
    cursor_to_pos(y + 3, x - 18);
    printf("|        2 : samaneh taghzie       |");
    cursor_to_pos(y + 4, x - 18);
    printf("|           3 : log out            |");
    cursor_to_pos(y + 5, x - 18);
    printf("|__________________________________|");
    cursor_to_pos(y * 4, x);
    while (true) {
        char key = nb_getch();
        if(key != 0)
        {
            cout << key << '\n';
        delay(1000);
        if (key == '1')
            profile();
        else if (key == '2')
            samaneh();
        else if (key == '3')
            welcom_page();
        else   
            cursor_to_pos(y * 4, x);
        } 
    }
}
void sign_up() 
{
    string confirm_str;
    cls();
    do 
    {
        if (!user[user_cnt].id.empty())
        {
            cout << "ID ALRADY TAKEN\nPLEASE TRY AGAIN!\n\n\n";
            progress_bar(20, 100);
            cls();
        }
        cout << "PLEASE TELL US YOUR ID\n";
        cin >> user[user_cnt].id;
    }
    while (find_user(user[user_cnt].id));
    do 
    {
        cout << "enter your PASSWORD !!\n";
        hide_text();
        cin >> user[user_cnt].password;
        default_text();
        cout << "COMFIRM YOUR PASSWORD\n";
        hide_text();
        cin >> confirm_str;
        default_text();
        if (confirm_str != user[user_cnt].password)
            cout << "WORNG ! please try again\n";
    }
    while (confirm_str != user[user_cnt].password);
    cout << "ENTER YOUR FIRST AND LAST NAME\n";
    cin >> user[user_cnt].name >> user[user_cnt].last_name;
    cout << "ENTER YOUR PHONE NUMBER\n";
    cin >> user[user_cnt].phone_num;
    cout << "ENTER YOUR RESHTE :)\n";
    cin >> user[user_cnt].reshte;
    cout << "ENTER YOUR TERM\n";
    cin >> user[user_cnt].term;
    user[user_cnt].balance = 0;
    user[user_cnt].plan = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    save_account(user_cnt++);
    delay(500);
    cls();
    cout << "ACOUNT SUCESSFULY MADE\nWELCOME " << user[user_cnt - 1].name << ' ' << user[user_cnt - 1].last_name << " :)\n\n\n";
    progress_bar(30, 100);
    cls();
    login_page();
}
void log_in() 
{
    cls();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("_____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                   |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|    Enter username and password    |");
    cursor_to_pos(y + 3, x - 18);
    printf("|___________________________________|");
    cursor_to_pos(y * 4 - 3, 0);
    cout << "ID : ";
    cursor_to_pos(y * 4 - 3, 6);
    string un, ps;
    cin >> un;
    cursor_to_pos(y * 4 - 2, 0);
    cout << "PASSWORD : ";
    hide_text();
    cin >> ps;
    default_text();
    if (user[find_user(un)].password == ps)
    {
        logged_in_user = find_user(un);
        cls();
        system("color 20");
        x = get_window_cols() / 2, y =  get_window_rows() / 4;
        cursor_to_pos(y, x - 18);
        printf("______________________________________"); 
        cursor_to_pos(y + 1, x - 18);
        printf("|                                    |"); 
        cursor_to_pos(y + 2, x - 18);
        printf("|        LOGIN WAS SUCCESSFUL        |");
        cursor_to_pos(y + 3, x - 18);
        printf("|____________________________________|");
        print_username();
        cursor_to_pos(y *3, x - 10);
        progress_bar(18, 100);
        cls();  
        home_page();
    }
    cls();
    x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("______________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                    |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|      ID OR PASSWORD WAS WRONG      |");
    cursor_to_pos(y + 3, x - 18);
    printf("|____________________________________|");
    cursor_to_pos(y *3, x - 10);
    progress_bar(18, 200);
    log_in();
}
void admin_login()
{
    a_cls();
    if (!admin_is) {
        string ps;
        cout << "ADMIN ACOUNT DOESN'T EXIST\nSO MAKE ONE!\nENTER YOUR ID :";
        cin >> admin.id;
        cout << "ENTER YOUR PASSWORD :";
        hide_text();
        cin >> admin.password;
        admin_text();
        cout << "CONFIRM YOUR PASSWORD :";
        hide_text();
        cin >> ps;
        while (ps != admin.password)
        {
           admin_text();
            cout << "WORNG! TRY AGAIN\nENTER YOUR PASSWORD :";
            hide_text();
            cin >> admin.password;
            admin_text();
            cout << "CONFIRM YOUR PASSWORD :";
            hide_text();
            cin >> ps; 
        }
        save_admin();
        admin_is = 1;
        login_page();
    }
    string ps, id;
    cout << "ENTER YOUR ID :";
    cin >> id;
    cout << "ENTER YOUR PASSWORD :";
    hide_text();
    cin >> ps;
    admin_text();
    if (id != admin.id || ps != admin.password) {
        cout << "ID OR PASSWORD WAS WORNG\n";
        esc_bar();
        a_cls();
        admin_login();
    }
    admin_page();
}
void welcom_page() 
{
    cls();
    //system("start WiiShopChannelMusic.mp3");
    system("Color 50");
    int x = get_window_cols() / 2 - 35, y =  get_window_rows() / 2 - 4;
    //sbu selfservice
    cursor_to_pos(y, x);
    change_color(6);
    printf(" _____ _____ _____                                                     ");
    cursor_to_pos(y + 1, x);
    printf("|   __| __  |  |  |                                                    ");
    cursor_to_pos(y + 2, x);
    printf("|__   | __ -|  |  |                                                    ");
    cursor_to_pos(y + 3, x);
    printf("|_____|_____|_____|                                                    ");
    cursor_to_pos(y + 4, x);
    printf(" _____ _____ __    _____     _____ _____ _____ _____ _____ _____ _____ ");
    cursor_to_pos(y + 5, x);
    printf("|   __|   __|  |  |   __|___|   __|   __| __  |  |  |     |     |   __|");
    cursor_to_pos(y + 6, x);
    printf("|__   |   __|  |__|   __|___|__   |   __|    -|  |  |-   -|   --|   __|");
    cursor_to_pos(y + 7, x);
    printf("|_____|_____|_____|__|      |_____|_____|__|__|\\___/|_____|_____|_____|");
    cursor_to_pos(y + 9, x + 5);
    printf("by Barbodak");
    esc_bar();
    cls();
    login_page();
}
void login_page() 
{
    cls();
    int x = get_window_cols() / 2, y =  get_window_rows() / 4;
    cursor_to_pos(y, x - 18);
    printf("____________________________________"); 
    cursor_to_pos(y + 1, x - 18);
    printf("|                                  |"); 
    cursor_to_pos(y + 2, x - 18);
    printf("|        Enter 1 to LOG IN         |");
    cursor_to_pos(y + 3, x - 18);
    printf("|        Enter 2 to SIGN UP        |");
    cursor_to_pos(y + 4, x - 18);
    printf("|  Enter 3 to use the ADMIN panle  |");
    cursor_to_pos(y + 5, x - 18);
    printf("|__________________________________|");
    cursor_to_pos(y * 4, x);
    while (true) {
        char key = nb_getch();
        if(key != 0)
        {
            cout << key << '\n';
        delay(1000);
        if (key == '1')
            log_in();
        else if (key == '2')
            sign_up();
        else if (key == '3')
            admin_login();
        else   
            cursor_to_pos(y * 4, x);
        } 
    }
    cls();
}

void init() 
{
// initializes  program
    // calls Clui
    init_clui();
    system("mkdir accounts");
    system("mkdir foods");
    srand(time(NULL));
    load_files();
    cout << user_cnt << '\n';
    // hold();
    cls();
    welcom_page();
}

int main() 
{
    init();
} 