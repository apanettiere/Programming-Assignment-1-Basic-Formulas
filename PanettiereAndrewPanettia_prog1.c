#include <stdio.h>
#include <math.h>

int main() {
    int n;
    double R, ha, hb;
    double a, b, h, totalSurfaceArea, volume;
    double totalSurfaceAreaSum = 0, volumeSum = 0;
    const double PI = 3.14159265359;

    // Prompt for the number of segments
    printf("How many spherical segments you want to evaluate [2-10]? \n");
    scanf("%d", &n);
    while (n < 2 || n > 10) {
        printf("How many spherical segments you want to evaluate [2-10]? \n");
        scanf("%d", &n);
    }

    for (int i = 1; i <= n; i++) {
        printf("Obtaining data for spherical segment number %d\n", i);
        int validInput = 0;

        while (!validInput) {
            // Input values
            printf("What is the radius of the sphere (R)? \n");
            scanf("%lf", &R);
            printf("What is the height of the top area of the spherical segment (ha)? \n");
            scanf("%lf", &ha);
            printf("What is the height of the bottom area of the spherical segment (hb)? \n");
            scanf("%lf", &hb);
            printf("Entered data: R = %.2f ha = %.2f hb = %.2f.\n", R, ha, hb);

            // Validate input
            if (R <= 0 || ha <= 0 || hb <= 0 || R < ha || R < hb || ha < hb) {
                if (R <= 0) {
                    printf("Invalid Input: R = %.2f must be greater than 0.\n", R);
                } else if (ha <= 0 || hb <= 0) {
                    printf("Invalid Input: ha = %.2f and hb = %.2f must be greater than 0.\n", ha, hb);
                } else if (R < ha || R < hb) {
                    printf("Invalid Input: R = %.2f must be greater than or equal to ha = %.2f and hb = %.2f.\n", R, ha, hb);
                } else if (ha < hb) {
                    printf("Invalid Input: ha = %.2f must be greater than or equal to hb = %.2f.\n", ha, hb);
                }
                printf("Please re-enter data for this segment.\n");
            } else {
                validInput = 1;
            }
        }

        // Calculations
        a = sqrt(R * R - ha * ha);
        b = sqrt(R * R - hb * hb);
        h = ha - hb;
        double topSurfaceArea = PI * a * a;
        double bottomSurfaceArea = PI * b * b;
        double lateralSurfaceArea = 2 * PI * R * h;
        totalSurfaceArea = topSurfaceArea + bottomSurfaceArea + lateralSurfaceArea;
        volume = (1.0 / 6.0) * PI * h * (3 * a * a + 3 * b * b + h * h);

        printf("Total Surface Area = %.2f Volume = %.2f.\n", totalSurfaceArea, volume);

        // Accumulate for averages
        totalSurfaceAreaSum += totalSurfaceArea;
        volumeSum += volume;
    }

    // Calculate and display averages
    double averageSurfaceArea = totalSurfaceAreaSum / n;
    double averageVolume = volumeSum / n;

    // Report average results
    printf("Total average results:\n");
    printf("Average Surface Area = %.2f Average Volume = %.2f.\n", averageSurfaceArea, averageVolume);

    return 0;
}

