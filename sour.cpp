#include "head.h"
cl_base::cl_base(cl_base* p_prnt, string name)
{
    set_name(name);
    set_prnt(p_prnt);
}
//-------------------------------------
void cl_base::set_type_of_cl(int type_of_cl)
{
    this->type_of_cl = type_of_cl;
}

int cl_base::get_type_of_cl()
{
    return type_of_cl;
}
//-------------------------------------
void cl_base::set_name(string name)
{
    this->name = name;
}

string cl_base::get_name()
{
    return name;
}
//-------------------------------------
void cl_base::set_prnt(cl_base* p_prnt)
{
    if (p_prnt) {
        this->p_prnt = p_prnt;
        p_prnt -> add_chld(this);
    }
    else {
        this->p_prnt = p_prnt;
    }
}

cl_base* cl_base::get_prnt()
{
    return p_prnt;
}
//-------------------------------------
void cl_base::add_chld(cl_base* p_chld)
{
    children.push_back(p_chld);
}

cl_base* cl_base::get_any_chld(string name)
{
    cl_base* res = nullptr;
    if (get_name() == name) {
        return this;
    }
    if (children.size() == 0) {
        return nullptr;
    }
    for (cl_base* chld: children) {
        if (chld->get_any_chld(name)) {
            res = chld->get_any_chld(name);
        }
    }
    return res;
}

cl_base* cl_base::get_my_chld(string name)
{
    for (cl_base* chld: children) {
        if (chld->get_name() == name) {
            return chld;
        }
    }
    return nullptr;
}
//-------------------------------------
void cl_base::show_object_tree()
{
    int level = 0;
    show_object_next(this, level);
}

void cl_base::show_object_next(cl_base* p_prnt, int level)
{
    string space;
    if (level > 0) {
        space.append(4 * level, ' ');
    }
    if (p_prnt->get_prnt() == nullptr) {
        cout << space << p_prnt->get_name();
    }
    else {
        cout << endl << space << p_prnt->get_name();
    }
    if (p_prnt->children.size() == 0) {
        return;
    }
    for (cl_base* chld: p_prnt->children) {
        chld->show_object_next(chld, level + 1);
    }
}

void cl_base::prepare_tree()
{
    set_state(1);
    for (cl_base* chld: children) {
        chld->prepare_tree();
    }
}
//-------------------------------------
int cl_base::get_state()
{
    return state;
}

void cl_base::set_state(int state)
{
    if (state == 0) {
        this->state = 0;
        for (int i = 0; i < children.size(); i++) {
            children[i]->set_state(0);
        }
        return;
    }
    if (state) {
        if (p_prnt == nullptr) {
            this->state = state;
            return;
        }
        else if (p_prnt->get_state()) {
            this->state = state;
            return;
        }
        else {
            this->state = 0;
            return;
        }
    }
    return;
}

void cl_base::show_object_readiness(int level)
{
    cout << endl;
    for (int i = 0; i < level; i++) {
        cout << "    ";
    }
    cout << name << " is ";
    if (get_state() == 0) {
        cout << "not ";
    }
    cout << "ready";
    for (int i = 0; i < level; i++) {
        children[i]->show_object_readiness(level + 1);
    }
    return;
}
//-------------------------------------
cl_base* cl_base::find_object(string adress) {
    cl_base* res = nullptr;
    vector<string> object_names;
    string cur_obj_name;
    if (adress == "/") {
        return this;
    }
    if ((adress[0] == '/')&(adress[1] == '/')) {
        for (int i = 2; i < adress.length(); i++) {
            cur_obj_name += adress[i];
        }
        return get_any_chld(cur_obj_name);
    }
    if (adress[0] != '/') {
        for (int i = 0; i < adress.length(); i++) {
            if (adress[i] != '/') {
                cur_obj_name += adress[i];
            }
            else {
                object_names.emplace_back(cur_obj_name);
                cur_obj_name = "";
            }
        }
        object_names.emplace_back(cur_obj_name);
        for (int i = 0; i < object_names.size() - 1; i++) {
            if (get_any_chld(object_names[i])->get_my_chld(object_names[i+1])) {
                res = get_any_chld(object_names[i])->get_my_chld(object_names[i+1]);
            }
            else {
                res = nullptr;
                break;
            }
        }
    }
    if (adress[0] == '/') {
        object_names.emplace_back(get_name());
        for (int i = 1; i < adress.length(); i++) {
            if (adress[i] != '/') {
                cur_obj_name += adress[i];
            }
            else {
                object_names.emplace_back(cur_obj_name);
                cur_obj_name = "";
            }
        }
        object_names.emplace_back(cur_obj_name);
        for (int i = 0; i < object_names.size() - 1; i++) {
            if (get_any_chld(object_names[i])->get_my_chld(object_names[i+1])) {
                res = get_any_chld(object_names[i])->get_my_chld(object_names[i+1]);
            }
            else {
                res = nullptr;
                break;
            }
        }
    }
    return res;
}

cl_base* cl_base::find_root() {
    if (p_prnt) {
        return p_prnt->find_root();
    }
    else {
        return this;
    }
}

string cl_base::get_adress()
{
    if (p_prnt) {
        if (p_prnt->get_prnt()) {
            return p_prnt->get_adress() + '/' + name;
        }
        return p_prnt->get_adress() + name;
    }
    return string{'/'};
}
//-------------------------------------
void cl_base::set_connect(SIGNAL signal, cl_base* object, HANDLER handler)
{
    for (auto it = connects.begin(); it != connects.end(); it++) {
        if ((it->signal == signal) && (it->handler == handler) && (it->object == object)) {
            return;
        }
    }
    connect val;
    val.object = object;
    val.signal = signal;
    val.handler = handler;
    connects.push_back(val);
}

void cl_base::delete_connect(SIGNAL signal, cl_base* object, HANDLER handler)
{
    for (auto it = connects.begin(); it != connects.end(); it++) {
        if (it->object == object) {
            connects.erase(it);
            return;
        }
    }
}

void cl_base::emit_signal(SIGNAL signal, string& msg)
{
    if (get_state()) {
        HANDLER handler;
        (this->*signal)(msg);
        cl_base* root = find_root();
        for (auto it = root->connects.begin(); it != root->connects.end(); it++) {
            if (it->signal == signal) {
                handler = it->handler;
                if (it->object->get_state()){
                    (it->object->*handler)(msg);
                }

            }
        }
    }
}

SIGNAL cl_base::prepare_signal(int type_of_cl)
{
    if (type_of_cl == 1) {return SIGNAL_D(cl_app::signal);}
    if (type_of_cl == 2) {return SIGNAL_D(cl2::signal);}
    if (type_of_cl == 3) {return SIGNAL_D(cl3::signal);}
    if (type_of_cl == 4) {return SIGNAL_D(cl4::signal);}
    if (type_of_cl == 5) {return SIGNAL_D(cl5::signal);}
    if (type_of_cl == 6) {return SIGNAL_D(cl6::signal);}
    return nullptr;
}
HANDLER cl_base::prepare_handler(int type_of_cl)
{
    if (type_of_cl == 1) {return HANDLER_D(cl_app::handler);}
    if (type_of_cl == 2) {return HANDLER_D(cl2::handler);}
    if (type_of_cl == 3) {return HANDLER_D(cl3::handler);}
    if (type_of_cl == 4) {return HANDLER_D(cl4::handler);}
    if (type_of_cl == 5) {return HANDLER_D(cl5::handler);}
    if (type_of_cl == 6) {return HANDLER_D(cl6::handler);}
    return nullptr;
}



void cl_app::set_current(cl_base* current) {
    this->current = current;
}

cl_base* cl_app::get_current() {
    return current;
}

void cl_app::signal(string &msg) {
    cout << endl << "Signal from " << get_adress();
    msg += " (class: 1)";
}
void cl_app::handler(string msg) {
    cout << endl << "Signal to " << get_adress() << " Text:  " << msg;
}

void cl_app::bild_tree_objects()
{
    string name1, name2;
    int i;
    cl_base *from, *to;
    cin >> name1;
    set_name(name1);
    set_type_of_cl(1);
    cin >> name1;
    while (name1 != "endtree") {
        cin	>> name2 >> i;
        from = find_object(name1);
        if (from) {
            if (i == 2) {cl2* chld = new cl2(from, name2);chld->set_type_of_cl(i);}
            if (i == 3) {cl3* chld = new cl3(from, name2);chld->set_type_of_cl(i);}
            if (i == 4) {cl4* chld = new cl4(from, name2);chld->set_type_of_cl(i);}
            if (i == 5) {cl5* chld = new cl5(from, name2);chld->set_type_of_cl(i);}
            if (i == 6) {cl6* chld = new cl6(from, name2);chld->set_type_of_cl(i);}
        }
        else {
            cout << "Object tree" << endl;
            show_object_tree();
            cout << endl << "The head object " << name1 << " is not found";
            exit(0);
        }
        cin >> name1;
    }
    cin >> name1;
    while (name1 != "end_of_connections") {
        cin >> name2;
        from = find_object(name1);
        to = find_object(name2);
        SIGNAL sgnl = prepare_signal(from->get_type_of_cl());
        HANDLER hndl = prepare_handler(to->get_type_of_cl());
        set_connect(sgnl, to, hndl);
        cin >> name1;
    }
}

int cl_app::exec_app()
{
    string name1, name2, analyzer, copier;
    cl_base *from, *to;
    prepare_tree();
    cout << "Object_tree" << endl;
    show_object_tree();
    cin >> name1;
    while (name1 != "END") {
        cin >> name2 >> analyzer;
        copier.clear();
        while ((analyzer != "END") && (analyzer != "EMIT") && (analyzer != "SET_CONNECT") && (analyzer != "DELETE_CONNECT") && (analyzer != "SET_CONDITION")) {
            if (copier.size()) {
                copier += ' ';
            }
            copier += analyzer;
            cin >> analyzer;
        }
        from = find_object(name2);
        if (!from) {
            cout << endl << "Object " << name2 << " not found";
            name1 = analyzer;
            continue;
        }
        if (name1 == "EMIT") {
            from->emit_signal(prepare_signal(from->get_type_of_cl()), copier);
        }
        else if ((name1 == "SET_CONNECT") || (name1 == "DELETE_CONNECT")) {
            to = find_object(copier);
            if (to) {
                if (name1 == "SET_CONNECT") {
                    set_connect(prepare_signal(from->get_type_of_cl()), to, prepare_handler(to->get_type_of_cl()));
                }
                else {
                    delete_connect(prepare_signal(from->get_type_of_cl()), to, prepare_handler(to->get_type_of_cl()));
                }
            }
            else {
                cout << endl << "Handler object " << copier << " not found";
            }
        }
        else if (name1 == "SET_CONDITION") {
            from->set_state(stoi(copier));
        }
        name1 = analyzer;
    }
    return 0;
}