
#include <Rcpp.h>
using namespace Rcpp;

//' @name motif_census
//'
//' @title Compute motif census
//'
//' @description
//' Compute motif census
//'
//' @param mat a matrix of logical describing a network.
//'
//' @export
//'
//' @return
//' A dataframe with species as row and motifs as columns, ordered as follows:
//'	* 1 linear chains bottom
//'	* 2 linear chains middle
//'	* 3 linear chains top
//'	* 4 apparent compitition botton
//'	* 5 apparent compitition top
//'	* 6 expoitative competition bottom
//'	* 7 expoitative competition top
//'	* 8 omnivory bottom
//'	* 9 omnivory middle
//'	* 10 omnivory top

// [[Rcpp::export]]
NumericMatrix motif_census(LogicalMatrix mat) {

		NumericMatrix out(mat.nrow(), 10);
		// NB: below are how
		// 0-2 => linear chains => 0- bottom 1- medium 2- top
		// 3-4 => apparent competition => 3- bottom 4- top
		// 5-6 => expoitative competition => 5- bottom 6- top
		// 7-9 => omnivory => 7- bottom 8- medium -9 top
		int i, j, k;
		int nsp = mat.nrow();

		// remove interaction that are bidirectionnal and cannibalism
		for (i=  0; i < mat.nrow(); i++) {
				mat(i,i) = false;
				for (j = i+1; j < mat.ncol(); j++) {
						if (mat(i,j) && mat(j, i)) {
								mat(i,j) = false;
								mat(j,i) = false;
						}
				}
		}

		// Here start the part where interactions are checked
		for (i = 0; i < nsp-2; i++) {
				for (j = i+1; j < nsp-1; j++) {
						for (k = j+1; k < nsp; k++) {
								// there are 27 possibilities
								if (mat(i, j)) {
										//
										if (mat(j, k)) {
												if (mat(i, k)) {
														out(i, 9)++;
														out(j, 8)++;
														out(k, 7)++;
												} else if (mat(k, i)) {
														// circular
												} else {
														out(i, 2)++;
														out(j, 1)++;
														out(k, 0)++;
												}
										} else if (mat(k, j)) {
												if (mat(i, k)) {
														out(i, 9)++;
														out(j, 7)++;
														out(k, 8)++;
												} else if (mat(k, i)) {
														out(i, 8)++;
														out(j, 7)++;
														out(k, 9)++;
												} else {
														out(i, 6)++;
														out(j, 5)++;
														out(k, 6)++;
												}
										} else {
												if (mat(i, k)) {
														out(i, 4)++;
														out(j, 3)++;
														out(k, 3)++;
												} else if (mat(k, i)) {
														out(i, 1)++;
														out(j, 0)++;
														out(k, 2)++;
												} else {
														// not a motif
												}
										}
								} else if (mat(j, i)) {
										//
										if (mat(j, k)) {
												if (mat(i, k)) {
														out(i, 8)++;
														out(j, 9)++;
														out(k, 7)++;
												} else if (mat(k, i)) {
														out(i, 7)++;
														out(j, 9)++;
														out(k, 8)++;
												} else {
														out(i, 3)++;
														out(j, 4)++;
														out(k, 3)++;
												}
										} else if (mat(k, j)) {
												if (mat(i, k)) {
														// circular
												} else if (mat(k, i)) {
														out(i, 7)++;
														out(j, 8)++;
														out(k, 9)++;
												} else {
														out(i, 0)++;
														out(j, 1)++;
														out(k, 2)++;
												}
										} else {
												if (mat(i, k)) {
														out(i, 1)++;
														out(j, 2)++;
														out(k, 0)++;
												} else if (mat(k, i)) {
														out(i, 5)++;
														out(j, 6)++;
														out(k, 6)++;
												} else {
														// not a motif
												}
										}
								} else {
										//
										if (mat(j, k)) {
												if (mat(i, k)) {
														out(i, 6)++;
														out(j, 6)++;
														out(k, 5)++;
												} else if (mat(k, i)) {
														out(i, 0)++;
														out(j, 2)++;
														out(k, 1)++;
												} else {
														// no motif
												}
										} else if (mat(k, j)) {
												if (mat(i, k)) {
														out(i, 2)++;
														out(j, 0)++;
														out(k, 1)++;
												} else if (mat(k, i)) {
														out(i, 3)++;
														out(j, 3)++;
														out(k, 4)++;
												} else {
														// no motif
												}
										} else {
												// no motifs
										}
								}
						}
				}
		}

		return out;
}

