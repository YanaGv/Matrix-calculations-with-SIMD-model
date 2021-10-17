#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

int p, m, q, n,r;
//данный тип значения n необходим для предотвращения утечки данных
double n_double;

vector <vector <double> > Am, Bm, Em, Gm, Cm;
vector <vector<vector <double> > > Fijk, Dijk;
//счетчики операций
int counter_sum, 
	counter_diffference, 
	counter_multiplicity, 
	counter_comparing,
	counter_implicationA_B, 
	counter_implicationB_A, 
	counter_third_operation,
	counter_composition_Fijk,
	counter_composition_Dijk, 
	counter_calculate_Dijk;
//значений времени выполнения операций: сумма, разность, умножение, сравнение
int time_of_sum, 
	time_of_difference, 
	time_of_multiplicity, 
	time_of_comparing;
	
double Tn, Lsum, T1, Ky, e,Lavg;

//зануляет все значения
void grounding();
//генерирует матрицу размерностью rows x columns
vector <vector<double> > createMatrix(int rows, int columns);
void printMatrix(int rows, int columns, vector <vector<double> > matrix, string s);
//операции индивидуального задания
double composition_Fijk(int i, int j); //1
double composition_Dijk(int i, int j); //2 
double third_operation(int i, int j); //3
double implicationA_B(double a, double b); //4
double implicationB_A(double a, double b); //5
//шестая операция объединена с функцией вычисления матрицы D

//функции вычисления матриц
vector <vector<vector <double> > > calculate_Fijk();
vector <vector<vector <double> > > calculate_Dijk();
vector <vector<double> > calculate_Cm();
//вспомагательная функция генерации матриц
double random(double min, double max);

int main()
{		
	grounding();
	cout << "Enter p\n";
	cin >> p;
	cout << "Enter m\n";
	cin >> m;
	cout << "Enter q\n";
	cin >> q;
	cout << "Enter n\n";
	cin >> n;
	n_double = (double)n;
	
		
	cout << "Enter sum run time:\n";
	cin >> time_of_sum;
	cout <<"Enter difference run time:\n";
	cin >> time_of_difference;
	cout << "Enter multiplication run time:\n";
	cin >> time_of_multiplicity;
	cout << "Enter comparison run time:\n";
	cin >> time_of_comparing;
	
	cout << endl;

	Am = createMatrix(p, m);
	Bm = createMatrix(m, q);
	Em = createMatrix(1, m);
	Gm = createMatrix(p, q);
	Dijk = calculate_Dijk();
	Fijk = calculate_Fijk();
	Cm = calculate_Cm();

	printMatrix(p, m, Am, "Matrix A");
	printMatrix(m, q, Bm, "Matrix B");
	printMatrix(1, m, Em, "Matrix E");
	printMatrix(p, q, Gm, "Matrix G");
	printMatrix(p, q, Cm, "Matrix C");

	T1 = counter_comparing*time_of_comparing + 
		 counter_diffference*time_of_difference +
		 counter_multiplicity*time_of_multiplicity + 
		 counter_sum*time_of_sum;

	Lsum = Tn;
	Ky = T1 / Tn;
	e = Ky / n;
	r = p*q*m;
	
	Lavg += (7 * time_of_multiplicity + 2 * time_of_sum + 3 * time_of_difference) * p * q;
	Lavg += (7 * time_of_multiplicity + 2 * time_of_sum + 3 * time_of_difference) * r;
	Lavg += time_of_multiplicity *(m - 1) * counter_composition_Fijk;
	Lavg += (time_of_multiplicity*(m - 1)+ time_of_difference*(m+1))*counter_composition_Dijk;
	Lavg += time_of_comparing * counter_third_operation;	
	Lavg += (time_of_comparing + time_of_difference)*counter_implicationA_B;
	Lavg += (time_of_comparing + time_of_difference)*counter_implicationB_A;
	Lavg += time_of_comparing*counter_calculate_Dijk;
	Lavg /= r;

	double D = Tn / Lavg;
	
	cout << "\nT1 = " << T1 << "\nTn = " << Tn << "\nKy = " << Ky << "\ne = " << e << "\nLsum = " << Lsum
		<< "\nLavg = " << Lavg<<"\nD= "<<D<<endl<<endl;
	goto start;
    return 0;
}



vector <vector<double> > createMatrix(int rows, int columns) { 
	vector< vector<double> > matrix;
	vector <double> vector_row;
	int i;
	for (i = 0; i < columns; i++)  vector_row.push_back(0);

	for (int row = 0; row < rows; row++) {
		matrix.push_back(vector_row);
		for (int column = 0; column < columns; column++) {
			matrix[row][column] = random(-1,1);
		}
	}
	return matrix;
}

double random(double min, double max) {
	double f = (double)rand() / RAND_MAX;
	return min + f*(max - min);
}

void printMatrix(int rows, int columns, vector<vector<double> > matrix, string s) {
	cout << s << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << matrix[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl;
}

vector <vector<vector <double> > > calculate_Fijk() {

	vector <vector<vector <double> > > Fijk;
	vector <vector<double> > Fjk;
	vector <double> Fk;

	for (int k = 0; k < m; k++) Fk.push_back(0);
	for (int j = 0; j < q; j++) Fjk.push_back(Fk);
	for (int i = 0; i < p; i++) Fijk.push_back(Fjk);

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < q; j++) {
			for (int k = 0; k < m; k++) {
				Fijk[i][j][k] = implicationA_B(Am[i][k], Bm[k][j])*(2 * Em[0][k] - 1)*Em[0][k] 
					+implicationB_A(Am[i][k], Bm[k][j])
					*(1 + (4 * implicationA_B(Am[i][k], Bm[k][j]) - 2)*Em[0][k])*(1 - Em[0][k]);
				counter_multiplicity += 7;
				counter_diffference += 3;
				counter_sum += 2;
			}
		}
	}
 
	Tn += (7 * time_of_multiplicity + 2 * time_of_sum + 3 * time_of_difference) * ceil(p * q * m / n_double);;
	return Fijk;
}

vector <vector<vector <double> > > calculate_Dijk() {

	vector <double> Dk;
	vector < vector < double > > Djk;
	vector <vector<vector <double> > > Dijk;


	for (int k = 0; k < m; k++) Dk.push_back(0);
	for (int j = 0; j < q; j++) Djk.push_back(Dk);
	for (int i = 0; i < p; i++) Dijk.push_back(Djk);

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < q; j++) {
			for (int k = 0; k < m; k++) {
				Dijk[i][j][k] = min(Am[i][k], Bm[k][j]);
				counter_comparing++;
			}
		}
	}

	Tn += time_of_comparing * ceil(p * q * m / n_double);
	return Dijk;
}

vector <vector<double> > calculate_Cm() {

	vector <vector<double> > Cm;
	vector <double> Cj;

	for (int j = 0; j < q; j++) Cj.push_back(0);
	for (int i = 0; i < p; i++) Cm.push_back(Cj);


	for (int i = 0; i < p; i++) {
		for (int j = 0; j < q; j++) {
			Cm[i][j] = composition_Fijk(i, j)*
						(3 * Gm[i][j] - 2) *
						Gm[i][j] + 
						(composition_Dijk(i, j) +
						(4 * third_operation(i, j) + 3 * composition_Dijk(i, j)) * Gm[i][j])*
						(1 - Gm[i][j]);
						
			counter_multiplicity += 7;
			counter_diffference += 3;
			counter_sum += 2;
		}
	}
	
	Tn += (7 * time_of_multiplicity + 2 * time_of_sum + 3 * time_of_difference) * ceil(p* q / n_double);
	return Cm;
}

double implicationA_B(double a, double b) {

	counter_diffference++;
	counter_comparing++;
	counter_implicationA_B++;
	Tn += (time_of_comparing + time_of_difference) / n_double;	

	return max(1- a, b);
}

double implicationB_A(double a, double b) {

	counter_diffference++;
	counter_comparing++;
	counter_implicationB_A++;
	Tn += (time_of_comparing + time_of_difference) / n_double;

	return max(1 - b, a);
}

double composition_Fijk(int i, int j) {
	double result = 1;

	for (int k = 0; k < m; k++) {
		result *= Fijk[i][j][k];
		counter_multiplicity++;
	}

	counter_multiplicity -= m;
	counter_composition_Fijk++;
	Tn += time_of_multiplicity * (m - 1)/n_double;
	
	return result;
}

double composition_Dijk(int i, int j) {
	double result = 1;

	for (int k = 0; k < m; k++) {
		result *= 1 - Dijk[i][j][k];
		counter_diffference++;
		counter_multiplicity++;
	}
	
	counter_multiplicity -= 1;
	Tn += (time_of_multiplicity * (m - 1) + time_of_difference * m) / n_double;

	counter_composition_Dijk++;

	return 1 - result;
}

double third_operation(int i, int j) {

	counter_comparing++;
	counter_third_operation++;
	Tn += time_of_comparing;
	
	return min(composition_Fijk(i, j), composition_Dijk(i, j));
}

void grounding(){
	counter_sum=0; 
	counter_diffference=0;
	counter_multiplicity=0; 
	counter_comparing=0;

	counter_implicationA_B = 0; 
	counter_implicationB_A = 0; 
	counter_third_operation = 0;
	counter_composition_Fijk = 0;
	counter_composition_Dijk = 0; 
	counter_calculate_Dijk = 0;

	time_of_sum = 1;
	time_of_difference = 1;
	time_of_multiplicity = 1; 
	time_of_comparing = 1;
	
	Tn = 0; Lsum = 0; T1 = 0; Ky = 0; e = 0; Lavg=0;
	
	Am.clear();
	Bm.clear();
	Em.clear();
	Gm.clear();
	Dijk.clear();
	Fijk.clear();
	Cm.clear();
}
