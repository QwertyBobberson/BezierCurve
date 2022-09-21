# BezierCurve

Includes a function for drawing Bezier curves from a set of control points, along with one for creating a set of command points from a list of other points

Inorder to generate command points, enter an amount of command points you'd like to use to estimate your sample data, along with an accuracy to target.

The error is the average of the square of the distance between the sample points and the same points on the curve.
Sample data points are assumed to be spaced equally, and there must be atleast as many sample data points as requested control points.
