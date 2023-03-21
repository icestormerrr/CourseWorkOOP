#ifndef CL_BASE_H
#define CL_BASE_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class cl_base;
typedef void(cl_base::* SIGNAL) (string&);
typedef void(cl_base::* HANDLER) (string);
#define SIGNAL_D(signal) (SIGNAL) (&signal)
#define HANDLER_D(handler) (HANDLER) (&handler)

class cl_base {
private:
    cl_base* p_prnt;
    string name;
    int state;
    int type_of_cl;
    struct connect {
        cl_base* object;
        SIGNAL signal;
        HANDLER handler;
    };
    vector <connect> connects;
    vector <cl_base*> children;
public:
    cl_base(cl_base* p_prnt = nullptr, string name = "object");

    void set_type_of_cl(int type_of_cl);
    int get_type_of_cl();

    void set_name(string name);
    string get_name();

    void set_prnt(cl_base* p_prnt);
    cl_base* get_prnt();

    void add_chld(cl_base* p_prnt);
    cl_base* get_my_chld(string name);
    cl_base* get_any_chld(string name);

    void show_object_tree();
    void show_object_next(cl_base* p_prnt, int level);
    void prepare_tree();

    int get_state();
    void set_state(int state);
    void show_object_readiness(int level);

    cl_base* find_object(string adress);
    string get_adress();

    void set_connect(SIGNAL signal, cl_base* object, HANDLER handler);
    void delete_connect(SIGNAL signal, cl_base* object, HANDLER handler);
    void emit_signal(SIGNAL signal, string& msg);

    SIGNAL prepare_signal(int type_of_cl);
    HANDLER prepare_handler(int type_of_cl);

    cl_base* find_root();

};


class cl_app:public cl_base {
private:
    cl_base* current = this;
public:
    cl_app(cl_base* p_prnt = nullptr):cl_base(p_prnt) {}
    void set_current(cl_base* current);
    cl_base* get_current();
    void bild_tree_objects();
    int exec_app();
    void signal(string &msg);
    void handler(string msg);
};

class cl2 :public cl_base {
public:
    cl2(cl_base* p_prnt = nullptr, string name = "Object_root"):cl_base(p_prnt, name) {}
    void signal(string &msg) {
        //if (get_state()) {
            cout << endl << "Signal from " << get_adress();
            msg += " (class: 2)";
        //}
    }
    void handler(string msg) {
        //if (get_state()){
            cout << endl << "Signal to " << get_adress() << " Text: " << msg;
       // }
    }
};

class cl3 :public cl_base {
public:
    cl3(cl_base* p_prnt = nullptr, string name = "Object_root"):cl_base(p_prnt, name) {}
    void signal(string &msg) {
        //if (get_state()) {
            cout << endl << "Signal from " << get_adress();
            msg += " (class: 3)";
        //}
    }
    void handler(string msg) {
        //if (get_state()){
            cout << endl << "Signal to " << get_adress() << " Text: " << msg;
        //}
    }
};

class cl4 :public cl_base {
public:
    cl4(cl_base* p_prnt = nullptr, string name = "Object_root"):cl_base(p_prnt, name) {}
    void signal(string &msg) {
        //if (get_state()) {
            cout << endl << "Signal from " << get_adress();
            msg += " (class: 4)";
        //}
    }
    void handler(string msg) {
        //if (get_state()){
            cout << endl << "Signal to " << get_adress() << " Text: " << msg;
        //}
    }
};

class cl5 :public cl_base {
public:
    cl5(cl_base* p_prnt = nullptr, string name = "Object_root"):cl_base(p_prnt, name) {}
    void signal(string &msg) {
        //if (get_state()) {
            cout << endl << "Signal from " << get_adress();
            msg += " (class: 5)";
        //}
    }
    void handler(string msg) {
       // if (get_state()){
            cout << endl << "Signal to " << get_adress() << " Text: " << msg;
        //}
    }
};

class cl6 :public cl_base {
public:
    cl6(cl_base* p_prnt = nullptr, string name = "Object_root"):cl_base(p_prnt, name) {}
    void signal(string &msg) {
        //if (get_state()) {
            cout << endl << "Signal from " << get_adress();
            msg += " (class: 6)";
        //}
    }
    void handler(string msg) {
       // if (get_state()){
            cout << endl << "Signal to " << get_adress() << " Text: " << msg;
       // }
    }
};
#endif