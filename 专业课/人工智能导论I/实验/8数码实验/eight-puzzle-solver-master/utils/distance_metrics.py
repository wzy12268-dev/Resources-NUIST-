import math

def manhattan_distance(point1_x, point1_y, point2_x, point2_y):
    """ It is the sum of absolute values of differences in the point 1's x and y coordinates and the
        point 2's x and y coordinates respectively """
    return abs(point1_x - point2_x) + abs(point1_y-point2_y)

def eculidean_distance(point1_x, point1_y, point2_x, point2_y):
    return math.sqrt(math.pow(point1_x - point2_x,2) + math.pow(point1_y - point2_y,2))
