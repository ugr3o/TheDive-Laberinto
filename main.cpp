#include <iostream> 
#include <vector>
#include <random>
#include <queue>
#include <chrono>

using namespace std;
using namespace chrono;

struct Punto {
    int fila;
    int col;
};

mt19937 rnd(high_resolution_clock::now().time_since_epoch().count()); // generador aleatorio de alta precision

long long medirTiempo(high_resolution_clock::time_point inicio){ // la funcion recibe un punto en el tiempo como parametro, longlong es un numero grande como los microsegundos
    auto fin = high_resolution_clock::now();
    return duration_cast<microseconds>(fin - inicio).count();
}
// inicializa la matriz con '#' en todas las celdas
vector<vector<char>> crearTablero(int filas, int cols){
        vector<vector<char>> tablero(filas, vector<char>(cols, '#')); // el tamanho es dinamico segun el input del usuario
        return tablero;
    }
//funcion para imprimir el tablero en consola
void imprimirTablero(vector<vector<char>>& tablero,int filas,int cols){

     for (int i =0; i< filas; i++){
        for (int j= 0; j < cols; j++){
            cout << tablero[i][j] << " ";
        }
        cout << endl; // genera el salto de linea entre las filas
    }
}
//funcion para mostrar ubicacion de entrada y salida dentro del tablero
void entradaSalida(vector<vector<char>>& tablero, int filas, int cols){
    tablero[0][0] = 'E';
    tablero[filas -1][cols -1] = 'S';
    tablero[0][1] = ' '; // espacio al lado de la entrada
    tablero[filas-2][cols-1] = ' ';// espacio al lado de la salida
}
//DFS recursivo con backtraking 
void generarLaberinto(vector<vector<char>>& tablero, int fila, int col, int filas, int cols){
    int orden[] = {0,1,2,3};      // Fisher-Yates shuffle mezcla las direcciones para que sea aleatorio
    for(int i =3; i > 0; i--){
        int j = rnd() % (i + 1);
        swap(orden[i],orden[j]);
    }
    int df[] = {-2,+2,0,0};      // avanza de 2 en 2 para mantener paredes entre celdas
    int dc[] = {0,0,-2,+2};
    for(int i =0; i < 4; i++){
        int nuevaFila = fila + df[orden[i]];
        int nuevaCol = col + dc[orden[i]];

        if(nuevaFila >= 0 && nuevaFila < filas && nuevaCol >= 0 && nuevaCol < cols && tablero[nuevaFila][nuevaCol] == '#'){ // cuando no puede avanzar, retrocede automaticamente por la recursion
            tablero[fila + df[orden[i]]/2][col + dc[orden[i]]/2] = ' ';
            tablero[nuevaFila][nuevaCol] = ' ';
            generarLaberinto(tablero, nuevaFila, nuevaCol, filas, cols);
        }
    }
}
// BFS encuentra el camino mas corto de E a S
void encontrarSalida(vector<vector<char>>& tablero,int fila, int col, int filas, int cols){
   queue<Punto> explorar;
   vector<vector<bool>> visitado(filas,vector<bool>(cols,false));
   vector<vector<Punto>> padre(filas,vector<Punto>(cols,{-1,-1})); // poner un valor inexistente
   explorar.push({fila,col});
   visitado[fila][col]= true;

   while(!explorar.empty()){
        Punto actual = explorar.front(); //utilizamos "Punto" el struct, porque "actual" tiene que guardar las coordendas, fila y col
        explorar.pop();
        if (actual.fila == filas -1 && actual.col == cols -1){ // al final del recorrido si ya se encuentra en la salida se deja de explorar los vecinos
            break;
        }
        int df[] = {-1,+1,0,0};
        int dc[] = {0,0,-1,+1};
        for(int i = 0; i < 4; i++){
            int nuevaFila = actual.fila + df[i];
            int nuevaCol = actual.col + dc[i];
            if(nuevaFila >= 0 && nuevaFila < filas && nuevaCol >= 0 && nuevaCol < cols && tablero[nuevaFila][nuevaCol] != '#' && !visitado[nuevaFila][nuevaCol]){
                visitado[nuevaFila][nuevaCol] = true;
                padre[nuevaFila][nuevaCol] = {actual.fila,actual.col}; //la matriz padre guarda de donde vino cada celda
                explorar.push({nuevaFila,nuevaCol});
            }
        }
   }
   Punto actual = {filas-1,cols-1};       // al llegar a S reconstruye el camino siguiendo los padres hacia atras
   while(!(actual.fila == 0 && actual.col == 0)){
    if(tablero[actual.fila][actual.col] != 'S'){
        tablero[actual.fila][actual.col] = '*';
    }
    actual = padre[actual.fila][actual.col];
   }

}
//main donde llamare a todas las funciones
int main(){
    int filas = 0;
    int cols = 0;
    
    cout << "Ingrese el numero de filas: ";
    cin >> filas;
    cout << "Ingrese el numero de columnas: ";
    cin >> cols;

    if(filas < 5 || cols < 5){
    cout << "para una correcta generacion ingrese minimo 5 filas y 5 columnas" << endl;
    return 1;
    }

    if(filas % 2 == 0) filas++;
    if(cols % 2 == 0) cols++;

    vector<vector<char>> tablero = crearTablero(filas,cols);
    // chrono mide el tiempo en microsegundos
    auto inicio = high_resolution_clock::now(); // guarda el tiempo inicial antes de ejecutar
    generarLaberinto(tablero,1,1,filas,cols);   //es la ejecucion que se mide
    cout << "Tiempo generacion: " << medirTiempo(inicio) << " microsegundos" << endl; //calcula la diferencia

    entradaSalida(tablero,filas,cols);

    imprimirTablero(tablero,filas,cols);

    auto inicio2 = high_resolution_clock::now();
    encontrarSalida(tablero,0,0,filas,cols);
    cout << "Tiempo resolucion: " << medirTiempo(inicio2) << " microsegundos" << endl;

    imprimirTablero(tablero,filas,cols);
    return 0;
}