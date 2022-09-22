#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <math.h>

struct Vector
{
    double x;
    double y;

    Vector()
    {

    }

    Vector(double _x, double _y)
    {
        x = _x;
        y = _y;
    }


    double sqMagnitude()
    {
        return x * x + y * y;
    }

    double magnitude()
    {
        return sqrt(x * x + y * y);
    }

    Vector operator +(Vector other)
    {
        return Vector(x + other.x, y + other.y);
    }

    Vector operator +=(Vector other)
    {
        x += other.x;
        y += other.y;
        return Vector(x, y);
    }

    Vector operator -(Vector other)
    {
        return Vector(x - other.x, y - other.y);
    }

    Vector operator -=(Vector other)
    {
        x -= other.x;
        y -= other.y;
        return Vector(x, y);
    }

    Vector operator *(double scalar)
    {
        return Vector(x * scalar, y * scalar);
    }

    Vector operator /(double scalar)
    {
        return Vector(x / scalar, y / scalar);
    }

    bool operator ==(Vector other)
    {
        return x == other.x && y == other.y;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& obj)
    {
        return os << obj.x << "," << obj.y;
    }
};


Vector CreatePointsFromCurve(std::vector<Vector> weights, double distance);
std::vector<Vector> BezierCurveFromPoints(std::vector<Vector> points, double epsilon, int numPoints);
std::vector<Vector> BezierCurveFromPoints(std::vector<Vector> points, int numControls);
double CalculateError(std::vector<Vector> samplePoints, std::vector<Vector> testCurve);

Vector CreatePointsFromCurve(std::vector<Vector> weights, double t)
{
    int n = weights.size() - 1;
    Vector result = Vector(0, 0);
    std::vector<int> multipliers;
    for(int i = 0; i <= n; i++)
    {
        if(i == 0)
        {
            multipliers.push_back(1);
        }
        else
        {
            multipliers.push_back(multipliers[i - 1] * (n-(i - 1))/(i));
        }
        // std::cout << multipliers[i] << " " << n - i << " " << i << std::endl;
        result += weights[i] * multipliers[i] * pow(1 - t, n - i) * pow(t, i);
    }

    return result;
}

std::vector<Vector> BezierCurveFromPoints(std::vector<Vector> points, double epsilon, int numPoints)
{
    std::vector<Vector> controlPoints;
    double error;
    controlPoints = BezierCurveFromPoints(points, numPoints);
    error = CalculateError(points, controlPoints);
    double prevError = error + 1;
    std::cout << numPoints << " points gave an error of: " << error << std::endl;
    int it = 0;

    double originalScale = .001;
    double scale = originalScale;
    Vector prevDir = Vector(0, 0);
    double prevInd = -1;

    while(prevError - error > epsilon)
    {
        prevError = error;
        Vector direction;
        int index;
        double newBest = error;
        for(int i = 1; i < controlPoints.size() - 1; i++)
        {
            for(int y = -1; y < 2; y++)
            {
                for(int x = -1; x < 2; x++)
                {
                    if(x != 0 || y != 0)
                    {
                        controlPoints[i] += Vector(x, y) * scale;
                        double testError = CalculateError(points, controlPoints);

                        if(testError < newBest)
                        {
                            newBest = testError;
                            direction = Vector(x, y);
                            index = i;
                        }

                        controlPoints[i] -= Vector(x, y) * scale;
                    }
                }
            }
        }

        controlPoints[index] += direction * scale;

        error = CalculateError(points, controlPoints);
        // std::cout << "Moved point " << index << " by " << direction * scale << "\n" 
        //           << "After iteration " << it << " error is " << error << std::endl;
        prevDir = direction;
        prevInd = index;
        it++;
        // std::cout << error << " " << prevError << " " << epsilon  << " " << prevError - error << std::endl;
    }

    return controlPoints;
}


std::vector<Vector> BezierCurveFromPoints(std::vector<Vector> points, int numControls)
{
    Vector p0, p1, p2, p3;
    switch(numControls)
    {
        case 2:
            p0 = points[0];
            p1 = points[points.size() - 1];
            return {p0, p1};
            break;
        case 3:
            p0 = points[0];
            p1 = points[0] + ((points[1] - points[0]) * points.size()/2);
            p2 = points[points.size() - 1];
            return {p0, p1, p2};
            break;
        case 4:
            p0 = points[0];
            p1 = points[0] + ((points[1] - points[0]) * points.size()/3);
            p2 = points[points.size() - 1] - (((points[points.size() - 2]) - (points[points.size() - 1])) * points.size()/-3.0);
            p3 = points[points.size() - 1];
            return {p0, p1, p2, p3};
            break;
    }

    std::vector<Vector> controlPoints1;
    std::vector<Vector> controlPoints2;
    std::vector<Vector> points1;
    std::vector<Vector> points2;

    int offset = points.size() * 1.0/numControls;
    for(int i = 0; i < points.size() - offset; i++)
    {
        points1.push_back(points[i]);
        points2.push_back(points[i + offset]);
    }

    controlPoints1 = BezierCurveFromPoints(points1, numControls - 1);
    controlPoints2 = BezierCurveFromPoints(points2, numControls - 1);

    std::vector<Vector> controlPoints;
    controlPoints.push_back(controlPoints1[0]);
    for(int i = 1; i < numControls - 1; i++)
    {
        controlPoints.push_back((controlPoints1[i] * (1-(double)i/(numControls - 1))) + (controlPoints2[i - 1] * (double)i/(numControls - 1)));
    }
    controlPoints.push_back(controlPoints2[controlPoints2.size() - 1]);

    return controlPoints;
}

double CalculateError(std::vector<Vector> samplePoints, std::vector<Vector> testCurve)
{
    double maxError = 0;
    for(int i = 0; i < samplePoints.size(); i+=samplePoints.size()/testCurve.size()/2)
    {
        Vector testCurvePoint = CreatePointsFromCurve(testCurve, (double)i/samplePoints.size());
        Vector correctPoint = samplePoints[i];
        maxError += (testCurvePoint - correctPoint).sqMagnitude();
    }

    return maxError/(samplePoints.size()/(samplePoints.size()/testCurve.size()/2));
}