#ifndef LOGREG_V1_H_INCLUDED
#define LOGREG_V1_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define LEARNING_RATE 0.01
#define EPOCHS 1000
#define MAX_SAMPLES 1000
#define MAX_FEATURES 5
#define MAX_LINE_LENGTH 1024

// Sigmoid function
double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

// Predict function
double predict(double *coefficients, double *features, int num_features) {
    double z = coefficients[0]; // Intercept
    for (int i = 0; i < num_features; ++i) {
        z += coefficients[i + 1] * features[i];
    }
    return sigmoid(z);
}

// Train logistic regression model using gradient descent
void train_logistic_regression(double **X, double *y, double *coefficients, int num_samples, int num_features) {
    for (int epoch = 0; epoch < EPOCHS; ++epoch) {
        double gradients[num_features + 1];

        // Initialize gradients to 0
        for (int j = 0; j <= num_features; ++j) {
            gradients[j] = 0.0;
        }

        // Calculate gradients
        for (int i = 0; i < num_samples; ++i) {
            double prediction = predict(coefficients, X[i], num_features);
            double error = y[i] - prediction;

            gradients[0] += error; // Intercept gradient
            for (int j = 0; j < num_features; ++j) {
                gradients[j + 1] += error * X[i][j];
            }
        }

        // Update coefficients
        for (int j = 0; j <= num_features; ++j) {
            coefficients[j] += LEARNING_RATE * gradients[j] / num_samples;
        }
    }
}

// Load data from CSV file
int load_data_from_csv(const char *filename, double **X, double *y, int *num_samples, int *num_features) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    char *token;
    int row = 0;

    // Read header to determine number of features
    fgets(line, MAX_LINE_LENGTH, file);
    token = strtok(line, ",");
    *num_features = 0;
    while (token) {
        token = strtok(NULL, ",");
        (*num_features)++;
    }
    (*num_features)--;

    // Rewind file
    rewind(file);

    // Skip header
    fgets(line, MAX_LINE_LENGTH, file);

    // Read data
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        token = strtok(line, ",");
        X[row] = (double *)malloc(*num_features * sizeof(double));
        int col = 0;
        while ((token = strtok(NULL, ","))) {
            if (col < *num_features) {
                X[row][col++] = atof(token);
            } else {
                y[row] = atof(token); // Last column is the target variable
            }
        }
        row++;
    }

    *num_samples = row;

    fclose(file);
    return 0;
}



#endif // LOGREG_V1_H_INCLUDED
