#include <iostream>
#include <vector>
#include <complex>
#include <stdexcept>
using namespace  std;

template<typename T>
class Polynomial {
private:
    std::vector<T> coefficients;

public:
    // Конструктор с максимальной степенью
    Polynomial(size_t degree) : coefficients(degree + 1, T()) {}

    // Конструктор с вектором коэффициентов
    Polynomial(const std::vector<T>& coeffs) : coefficients(coeffs) {
        shrink_to_fit();
    }

    //Конструтор копирования
    Polynomial(const Polynomial& other) : coefficients(other.coefficients) {}

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            coefficients = other.coefficients;
        }
        return *this;
    }

    // Оператор [] для чтения коэффициента
    T operator[](size_t degree) const {
        if (degree >= coefficients.size()) {
            return T(); // Возвращает 0, если степень превышает текущую
        }
        return coefficients[degree];
    }

    // Метод set для установки коэффициента
    void set(size_t degree, T value) {
        if (degree >= coefficients.size()) {
            coefficients.resize(degree + 1, T());
        }
        coefficients[degree] = value;
        shrink_to_fit();
    }

    // Оператор сложения
    Polynomial operator+(const Polynomial& other) const {
        size_t max_degree = std::max(coefficients.size(), other.coefficients.size());
        Polynomial result(max_degree - 1);
        for (size_t i = 0; i < max_degree; ++i) {
            result.set(i, this->operator[](i) + other[i]);
        }
        return result;
    }

    // Оператор вычитания
    Polynomial operator-(const Polynomial& other) const {
        size_t max_degree = std::max(coefficients.size(), other.coefficients.size());
        Polynomial result(max_degree - 1);
        for (size_t i = 0; i < max_degree; ++i) {
            result.set(i, this->operator[](i) - other[i]);
        }
        return result;
    }

    // Оператор умножения на скаляр
    Polynomial operator*(const T& scalar) const {
        Polynomial result(coefficients.size() - 1);
        for (size_t i = 0; i < coefficients.size(); ++i) {
            result.set(i, coefficients[i] * scalar);
        }
        return result;
    }

    // Вычисление значения многочлена в точке x
    T evaluate(const T& x) const {
        T result = T();
        T power_of_x = T(1);
        for (size_t i = 0; i < coefficients.size(); ++i) {
            result += coefficients[i] * power_of_x;
            power_of_x *= x; // Увеличение степени x
        }
        return result;
    }

    // Метод shrink_to_fit
    void shrink_to_fit() {
        while (!coefficients.empty() && coefficients.back() == T()) {
            coefficients.pop_back(); // Удаление ведущих нулей
        }
    }

    // Метод expand
    void expand(size_t new_degree) {
        if (new_degree >= coefficients.size()) {
            coefficients.resize(new_degree + 1, T());
        }
    }

    // Метод для нахождения производной
    Polynomial derivative() const {
        Polynomial result(coefficients.size() - 2);
        for (size_t i = 1; i < coefficients.size(); ++i) {
            result.set(i - 1, coefficients[i] * static_cast<T>(i)); // Применение формулы производной
        }
        return result;
    }

    // вывод
    void print() const {
        for (size_t i = 0; i < coefficients.size(); ++i) {
            cout << coefficients[i] << "x^" << i;
            if (i < coefficients.size() - 1) cout << " + ";
        }
        cout << std::endl;
    }

    //Ввод
    void input() {
        cout << "Input coeffs (tap [q] to finish):" << endl;
        T coeff;
        size_t degree = 0;
        while (true){
            cout << "Coeff for x^" << degree << ": ";
            if (!(cin >> coeff)) {
                cin.clear();
                string input;
                cin >> input;
                if (input == "q") break;
                else {
                    cerr << "error" << endl;
                    continue;
                }
            }
            set(degree, coeff);
            ++degree;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

// Функция для тестирования многочлена
int main() {
    // Создание многочлена 1 - 3x^2 + 4x^3
    Polynomial<double> p({ 1, 0, -3, 4 });
    cout << "Polynomial p(x): ";
    p.print();

    // Вычисление и вывод производной многочлена
    Polynomial<double> dp = p.derivative();
    cout << "Derivative of polynomial p(x): ";
    dp.print();

    // Пример вычисления значения многочлена в точке x = 2
    double value = p.evaluate(2.0);
    cout << "Result p(2) = " << value << endl;

    return 0;
}
