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
    for(double i = 0; i < 1; i += 0.001)
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

    for(double i = 0; i < 1; i += 0.001)
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
    // DrawLine(-0.5, 0, 0.5, 0, .01, 0, 0, 1);

	// DrawSquare(-0.5, -0.5, 1, 1, 0, 0, 1);

	// PNG png = ReadPng("test2.png");

	// std::cout << png.height << " " << png.width << std::endl;

	// int sHeight = HEIGHT;
	// int sWidth = WIDTH;

	// std::cout << -1 * sHeight << std::endl;

	// DrawLine(-0.5, -0.5, glm::vec2(sqrt(2), sqrt(2)), 1, .01, 0, 0, 1);
	// DrawSquare(0, 0, -0.5, 1, 1, 0, 0, 0);

	// for(int y = -sHeight; y < sHeight; y++)
	// {
	// 	for(int x = -sWidth; x < sWidth; x++)
	// 	{
	// 		int xIndex = x == -sWidth ? 0 : floor((double)png.width/((x + sWidth)/2.0)) * 4;
	// 		int yIndex = y == -sHeight ? 0 : floor((double)png.height/((y + sHeight)/2.0));
	// 		std::cout << x << " " << y << std::endl;
	// 		std::cout << xIndex << " " << yIndex << std::endl;
	// 		double r = (png.pRows[yIndex][xIndex + 0])/255.0;
	// 		double g = (png.pRows[yIndex][xIndex + 1])/255.0;
	// 		double b = (png.pRows[yIndex][xIndex + 2])/255.0;

	// 		// double xPos = (double)x/png.width;
	// 		// std::cout << y << " " << xPos << " " << x << std::endl;

	// 		// double yPos = (double)y/png.height;
	// 		// std::cout << y << " " << yPos << " " << x << std::endl;

	// 		// double pWidth = 1.0/png.width;
	// 		// std::cout << y << " " << pWidth << " " << x << std::endl;

	// 		// double pHeight = 1.0/png.height;
	// 		// std::cout << y << " " << pHeight << " " << x << std::endl;

	// 		// if(r != 0 || g != 0 || b != 0)
	// 		// {
	// 		// 	std::cout << y << " " << r << " " << x << std::endl;
	// 		// 	std::cout << y << " " << g << " " << x << std::endl;
	// 		// 	std::cout << y << " " << b << " " << x << std::endl;
	// 		// }

	// 		// std::cout << ((double)x/WIDTH) << std::endl;
	// 		DrawSquare(((double)x/WIDTH), ((double)y/HEIGHT), 1.0/WIDTH, 1.0/HEIGHT, r, g, b);
	// 	}
	// }

	// std::cout << (int)png.colorType << std::endl;
	//   << png.pRows[png.height - 1][png.rows - 1][3] << ", ";

	// verts.resize(10);
	// indices.resize(10);
	
	// std::cout << "Validation Layers: " << enableValidationLayers<< std::endl;
	
	// PNG png = ReadPng("test.png");
	
	
	// verts = 
	// {
	// 	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	// 	{{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	// 	{{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	// 	{{-0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}
	// };

	// indices = 
	// {
	// 	0, 1, 2, 
	// 	2, 3, 0
	// };

	// DrawSquare(-0.5, -0.5, -0.5, 1, 1, 0, 0, 1);
	// // DrawSquare(-0.5, -0.5, 0, 1, 1, 0, 0, 1);
	// DrawSquare(-0.5, -0.5, 0.5, 1, 1, 0, 0, 1);
	// DrawSquare(-0.5, -0.5, 1, 1, 1, 0, 0, 1);

	// double size = std::stod(argv[1]);
	// for(int y = -size; y < size; y += 1)
	// {
	// 	for(int x = -size; x < size; x += 1)
	// 	{
	// 		if((x + y) % 2 == 0)
	// 		{
	// 			DrawSquare(x/size, y/size, 1/size, 1/size, 0, 0.05, 0.95);
	// 		}
	// 		else
	// 		{
	// 			DrawSquare(x/size, y/size, 1/size, 1/size, 0.005, 0.005, 0.005);
	// 		}
	// 	}
	// }
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