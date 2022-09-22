#include "Application.cpp"
#include "curve.cpp"
#include <typeinfo>
using namespace std;


void DrawSquare(double x, double y, double z, double width, double height, double r, double g, double b)
{
	int start = verts.size();
	verts.push_back({{x			, y			, z}, {r, g, b}, {0.0f, 0.0f}});
	verts.push_back({{x + width	, y			, z}, {r, g, b}, {1.0f, 0.0f}});
	verts.push_back({{x			, y + height, z}, {r, g, b}, {0.0f, 1.0f}});
	verts.push_back({{x + width	, y + height, z}, {r, g, b}, {1.0f, 1.0f}});

	indices.push_back(start + 0);
	indices.push_back(start + 1);
	indices.push_back(start + 2);
	indices.push_back(start + 2);
	indices.push_back(start + 3);
	indices.push_back(start + 1);
}

void DrawLine(double x1, double y1, glm::vec2 direction, double length, double width, double r, double g, double b)
{
    direction *= length;
    if(direction.x < 0)
    {
        x1 += direction.x;
        direction.x *= -1;
    }
    if(direction.y > 0)
    {

    }

	int start = verts.size();
	verts.push_back({{x1			            , y1			            , 0}, {r, g, b}, {0.0f, 0.0f}});
	verts.push_back({{x1 + direction.x	        , y1 + direction.y          , 0}, {r, g, b}, {1.0f, 0.0f}});
	verts.push_back({{x1 + width	            , y1 + width                , 0}, {r, g, b}, {0.0f, 1.0f}});
	verts.push_back({{x1 + direction.x + width	, y1 + direction.y + width  , 0}, {r, g, b}, {1.0f, 1.0f}});

	indices.push_back(start + 0);
	indices.push_back(start + 1);
	indices.push_back(start + 2);
	indices.push_back(start + 2);
	indices.push_back(start + 3);
	indices.push_back(start + 1);
}

void DrawLine(double x1, double y1, double x2, double y2, double width, double r, double g, double b)
{
    glm::vec2 direction(x2 - x1, y2 - y1);
    double length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= length;
    DrawLine(x1, y1, direction, length, width, r, g, b);
}

int main(int argc, char** argv)
{
	Application app;

   // CreatePointsFromCurve({{-.75, 0}, {-.5, .5}, {-.25, .5}, {.25, .5}, {.5, .5}, {.75, 0}}, .5);
    double accuracy = std::stod(argv[1]);


    std::vector<Vector> controlPointsOriginal;
    for(int i = 2; i < argc; i+=2)
    {
        controlPointsOriginal.push_back(Vector(std::stod(argv[i]), -std::stod(argv[i + 1])));
    }

    std::cout << "Read control points" << std::endl;
    // std::cout << controlPointsOriginal.size() << std::endl;

    std::vector<Vector> points;
    for(double i = 0; i < 1; i += 0.1)
    {
        Vector point = CreatePointsFromCurve(controlPointsOriginal, i);
        points.push_back(point);
    }
    std::cout << "Generated curve" << std::endl;

    for(int i = 0; i < points.size(); i++)
    {
        Vector point = points[i];
        DrawSquare(point.x, point.y, 0, .02, .02, 0, 0, 1);
    }

    std::cout << "Drew real curve" << std::endl;

    std::vector<Vector> controlPointsGenerated;
    
    if(accuracy > 0)
    {
        controlPointsGenerated = BezierCurveFromPoints(points, accuracy, (argc - 2)/2);
    }
    else
    {
        controlPointsGenerated = BezierCurveFromPoints(points, (argc - 2)/2);
    }
    std::vector<Vector> newPoints;

    std::cout << "Generate estimate control points" << std::endl;

    for(double i = 0; i < 1; i += 0.1)
    {
        Vector point = CreatePointsFromCurve(controlPointsGenerated, i);
        newPoints.push_back(point);
    }
    std::cout << "Generate estimate curve" << std::endl;

    for(int i = 0; i < newPoints.size(); i++)
    {
        Vector point = newPoints[i];
        DrawSquare(point.x, point.y, 0, .01, .01, 0, 1, 0);
    }

    for(int i = 0; i < controlPointsGenerated.size(); i++)
    {
        if(controlPointsGenerated[i].x > 1 || controlPointsGenerated[i].x < -1 || controlPointsGenerated[i].y > 1 || controlPointsGenerated[i].y < -1)
        {
            std::cout << i << " is at " << controlPointsGenerated[i] << std::endl;
        }
        DrawSquare(controlPointsGenerated[i].x, controlPointsGenerated[i].y, 0, .04, .04, 1, 0, 0);
        DrawSquare(controlPointsOriginal[i].x, controlPointsOriginal[i].y, 0, .03, .03, 1, 1, 1);
        // std::cout << controlPointsGenerated[i] << std::endl;
    }
	
	try
	{
		app.Run(verts, indices);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
	
}