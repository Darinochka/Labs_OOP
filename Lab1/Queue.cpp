#include "Queue.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


//Queue::Node::Node(MoveCommand data_new, Node* next_new) {
//    *data = data_new;
//    next = next_new;
//}
//
//Queue::Node::Node() {
//    *data = MoveCommand();
//    next = nullptr;
//};

Queue::Queue() {
    size = 0;
    head = nullptr;
    tail = nullptr;
};

Queue::Queue(const Queue& queue) {
    Node* curr = queue.head;

    while (curr->next != nullptr) {
        curr = curr->next;
        enqueue(*(curr->data));
    }
}

Queue::~Queue() {
    clear();
};

void Queue::clear() {
    while (size != 0) {
        Node* temp = head;
        head = head->next;
        delete temp;
        size--;
    }
    head = nullptr;
    tail = nullptr;
}

void Queue::enqueue(MoveCommand& data) {           // adds element to tail of queue
    MoveCommand *m;
    if (data.is_rotation()) {
        m = new Rotation(*(dynamic_cast<Rotation*>(&(data))));
    }
    else {
        m = new MoveCommand(data);
    }
    Node* p = new Node();
    p->data = m;

    if (tail == nullptr) {
        tail = p;
        head = new Node();
        head->next = tail;
    }
    else {
        tail->next = p;
        tail = tail->next;
    }
    size++;
};

Queue::Iterator Queue::begin() const {
    return Iterator(head->next);
}

Queue::Iterator Queue::end() const {
    return Iterator(tail->next);
}

void Queue::dequeue() {                          // delete from queue first element
    Node* current = head->next;
    MoveCommand* data = head->next->data;

    head = current;
    *head->data = MoveCommand();
    size--;
}

void Queue::load_file(ifstream& stream) {
    int time, move;
    string type_move;

    while (stream >> time >> type_move >> move) {
        if (type_move == "speed") {
            MoveCommand m(time, move);
            enqueue(m);
        }
        else if (type_move == "rotation") {
            Rotation m(time, move);
            enqueue(m);
        }
    }
}

void Queue::save_file(ofstream& stream) const {
    Node* current = head;

    while (current->next != nullptr) {
        current = current->next;
        MoveCommand& m = *current->data;
        stream << m;
    }
}
    
int Queue::get_size() const {
    return size;
};

vector <int> Queue::get_coord() const {
    Node* current = head;
    int x = 0, y = 0, time = 0;
    int speed = 0;

    while (current->next != nullptr) {
        current = current->next;
        MoveCommand& m = *current->data;
        int new_time = m.get_time();
        int radius = m.get_radius();
        int tempTime = new_time - time;

        if (m.is_rotation()) {
            int alpha = (speed / radius) * tempTime;
            x = round(radius * cos(alpha) + x);
            y = round(radius * sin(alpha) + y);
        }
        else {
            int new_speed = m.get_speed();
            int s = tempTime * new_speed;
            x = x + s;
            speed = new_speed;
        }
        time = new_time;
    }
    return { x, y };
}
bool Queue::operator==(const Queue& other) const {
    if ((head == nullptr && other.head != nullptr) ||
        (head != nullptr && other.head == nullptr)) {
        return false;
    }
    else if (head == nullptr && other.head == nullptr) {
        return true;
    }

    if (get_size() == other.get_size() &&
        begin() == other.begin()) {
        return true;
    }
    return false;
}

bool Queue::operator!=(const Queue& other) const{
    return !(*this == other);
}
