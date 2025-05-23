#include <iostream>
#include <vector>
#include <fstream>
#include <tuple>
#include <string>
#include <algorithm>

using namespace std;

struct Resultado {
    int maxScore;
    string subsecuenciaPrincipal;
    tuple<int, int> pos1;
    tuple<int, int> pos2;
    vector<vector<int>> matriz;
    vector<tuple<int, int, string>> otrasSubsecuencias;
};

Resultado smithWaterman(string s1, string s2, int match, int mismatch, int gap) {
    int m = s1.length(), n = s2.length();
    vector<vector<int>> H(m + 1, vector<int>(n + 1, 0));

    int maxScore = 0;
    vector<tuple<int, int>> coordsMax;

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int diag = H[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? match : mismatch);
            int up = H[i - 1][j] + gap;
            int left = H[i][j - 1] + gap;
            H[i][j] = max({0, diag, up, left});
            if (H[i][j] > maxScore) {
                maxScore = H[i][j];
                coordsMax.clear();
                coordsMax.emplace_back(i, j);
            } else if (H[i][j] == maxScore) {
                coordsMax.emplace_back(i, j);
            }
        }
    }

    auto retroceso = [&](int i, int j) {
        string subseq;
        int iniI = i, iniJ = j;
        while (i > 0 && j > 0 && H[i][j] > 0) {
            if (H[i][j] == H[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? match : mismatch)) {
                subseq = s1[i - 1] + subseq;
                --i; --j;
            } else if (H[i][j] == H[i - 1][j] + gap) {
                --i;
            } else {
                --j;
            }
        }
        return make_tuple(i, j, subseq, iniI - 1, iniJ - 1);
    };

    auto [i1, j1, subPrincipal, f1, f2] = retroceso(get<0>(coordsMax[0]), get<1>(coordsMax[0]));
    Resultado resultado;
    resultado.maxScore = maxScore;
    resultado.subsecuenciaPrincipal = subPrincipal;
    resultado.pos1 = {i1, f1};
    resultado.pos2 = {j1, f2};
    resultado.matriz = H;

    for (size_t k = 1; k < coordsMax.size(); ++k) {
        auto [a, b, sub, _, __] = retroceso(get<0>(coordsMax[k]), get<1>(coordsMax[k]));
        resultado.otrasSubsecuencias.emplace_back(a, b, sub);
    }

    return resultado;
}

void guardarResultado(const Resultado& res, const string& filename) {
    ofstream file(filename);

    file << "(i) Valor mayor (score máximo): " << res.maxScore << "\n";
    file << "(ii) Subsecuencia en común: " << res.subsecuenciaPrincipal << "\n";
    file << "(iii) Posición en la Secuencia 1: " << get<0>(res.pos1) << " a " << get<1>(res.pos1) << "\n";
    file << "(iii) Posición en la Secuencia 2: " << get<0>(res.pos2) << " a " << get<1>(res.pos2) << "\n";

    file << "\n(iv) Matriz de scores:\n";
    for (const auto& fila : res.matriz) {
        for (int val : fila) file << val << "\t";
        file << "\n";
    }

    file << "\n(v) ¿Existe alguna otra subsecuencia con el mismo valor?: ";
    if (res.otrasSubsecuencias.empty()) {
        file << "No\n";
    } else {
        file << "Sí\n";
        for (const auto& [i, j, subseq] : res.otrasSubsecuencias) {
            file << " - Subsecuencia: " << subseq << " desde posición " << i << "\n";
        }
    }

    file.close();
}

int main() {
    string seq1, seq2;
    int match, mismatch, gap;

    cout << "Ingrese la primera secuencia: ";
    cin >> seq1;
    cout << "Ingrese la segunda secuencia: ";
    cin >> seq2;
    cout << "Puntaje match (+): ";
    cin >> match;
    cout << "Penalización mismatch (-): ";
    cin >> mismatch;
    cout << "Penalización gap (-): ";
    cin >> gap;

    Resultado r = smithWaterman(seq1, seq2, match, mismatch, gap);
    guardarResultado(r, "resultado_alineamiento.txt");

    cout << "Resultado guardado en 'resultado_alineamiento.txt'\n";
    return 0;
}
