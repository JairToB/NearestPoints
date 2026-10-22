#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct list {
    double x;
    double y;
    int id;
};

double euclidian(list X, list Y) {
    double dx = X.x - Y.x;
    double dy = X.y - Y.y;
    double result = sqrt(dx * dx + dy * dy);
    return result;
}

double brutaF(std::vector<list>& X, int start, int end) {
    double minimum = std::numeric_limits<double>::infinity();

    for (int i = start; i <= end; ++i) {
        for (int j = i + 1; j <= end; ++j) {
            double result = euclidian(X[i], X[j]);

            if (result < minimum) {
                minimum = result;
            }
        }
    }

    return minimum;
}

double nearestPoints(std::vector<list>& X, std::vector<list>& Y, int start, int end) {

    if (end - start + 1 <= 3) {
        return brutaF(X, start, end);
    }

    int mid = start + (end - start) / 2;
    double xMid = (X[mid].x + X[mid + 1].x) / 2.0;
    std::vector<bool> belongs(X.size(), false);

    for (int i = start; i <= mid; ++i) {
        belongs[X[i].id] = true;
    }

    std::vector<list> leftY;
    std::vector<list> rightY;

    for (int i = 0; i < Y.size(); ++i) {
        if (belongs[Y[i].id]) {
            leftY.push_back(Y[i]);
        } else {
            rightY.push_back(Y[i]);
        }
    }

    double left = nearestPoints(X, leftY, start, mid);
    double right = nearestPoints(X, rightY, mid + 1, end);
    double delta = MIN(left, right);

    std::vector<list> strip;

    for (int i = 0; i < Y.size(); ++i) {
        if (std::abs(Y[i].x - xMid) < delta) {
            strip.push_back(Y[i]);
        }
    }

    double newFringe = delta;

    for (int i = 0; i < strip.size(); ++i) {
        for (int j = i + 1; j < strip.size() && strip[j].y - strip[i].y < newFringe; ++j) {
            double result = euclidian(strip[i], strip[j]);
            if (result < newFringe) {
                newFringe = result;
            }
        }
    }

    return newFringe;
}

void merge(std::vector<list>& arr, int left, int mid, int right, int coordinate) {

    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<list> L(n1);
    std::vector<list> R(n2);

    for (int i = 0; i < n1; ++i) {
        L[i] = arr[left + i];
    }

    for (int j = 0; j < n2; ++j) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        double a;
        double b;

        if (coordinate == 0) {
            a = L[i].x;
            b = R[j].x;
        } else {
            a = L[i].y;
            b = R[j].y;
        }

        if (a <= b) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }

        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(std::vector<list>& arr, int left, int right, int coordinate) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, coordinate);
        mergeSort(arr, mid + 1, right, coordinate);
        merge(arr, left, mid, right, coordinate);
    }
}

int main() {
    int n;
    std::cin >> n;

    std::vector<list> X(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> X[i].x >> X[i].y;
        X[i].id = i;
    }

    std::vector<list> Y = X;
    mergeSort(X, 0, n - 1, 0);
    mergeSort(Y, 0, n - 1, 1);

    double result = nearestPoints(X, Y, 0, n - 1);

    std::cout << "Distancia minima: " << result << std::endl;

    return 0;
}

