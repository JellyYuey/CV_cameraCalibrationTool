#include "worldcoordinatecalculator.h"

// Generate world coordinates for the chessboard
<<<<<<< HEAD

=======
>>>>>>> 10cf5d6e155af2e847060c1af42f091a5c3ddb82
std::vector<cv::Point3f> WorldCoordinateCalculator::generateWorldCoordinates(const std::vector<cv::Point2f>& imagePoints) {
    std::vector<cv::Point3f> worldPoints;

    // For the chessboard, assume the physical size of each square is boxSize (in millimeters)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Create world coordinates, assuming z=0, all points lie on the same plane
            worldPoints.push_back(cv::Point3f(j * boxSize, i * boxSize, 0.0f));
        }
    }
    worldCoordinates.push_back(worldPoints);
    return worldPoints;
}
