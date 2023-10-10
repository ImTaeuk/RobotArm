# RobotArm
## OpenGL을 활용한 프로젝트입니다.
### 프로젝트 설명
---
C++로 작성한 OpenGL을 이용한 컴퓨터그래픽스 결과물입니다.
로봇팔을 그리고, 로봇팔의 위치가 타겟의 위치에 가까워지도록 하는 기계학습 원리를 사용한 프로젝트입니다.

### 원리 설명
---
Gradient Descent를 통해 로봇 팔의 위치와 타겟과의 거리를 함수로 나타내는게 핵심입니다.
이 때, 거리를 식으로 나타낼 수 있는 방법은 로봇 팔의 위치를 (x, y)로 두고 타겟의 위치를 (target.x, target.y)로 둘 수 있습니다.
그러면 거리는 (target.x - x)^2 + (target.y - y)^2로 나타낼 수 있습니다.

이 때, 로봇팔의 위치는 로봇팔을 구성하는 3개의 팔 파츠가 있고 이것들이 지면과 이루는 각도가 각각 ang1, ang2, ang3로 있습니다.
따라서 (x, y) 는 위의 ang들로 정의가 되며 두 점 사이의 거리를 구함에 있어서 변수는 ang1, ang2, ang3가 됩니다. 따라서 이는 ang1, ang2, ang3로 구성되는 다변수 함수로 구성이 된다 할 수 있습니다.

이에 따라, Gradient Descent 원리에 의해 ang1이 타겟에 가까워지기 위해서는 거리를 의미하는 식을 ang들로 구성한 식에서 해당 ang 기준으로 편미분한 결과를 뺴주면 다음 ang 값을 알 수 있고 이 값을 통해 로봇 팔의 위치를 정해주면 됩니다.

## 코드 설명
### 헤더 설명
MyCube.h는 해당 프로젝트에서 정육면체를 그리기 위해 정의한 헤더입니다.

MyPyramid.h는 해당 프로젝트에서 피라미드를 그리기 위해 정의한 헤더입니다.

MyTarget.h는 MyCube.h 의 기능을 사용하지만, 추가로 위치 값을 제공하는 GetPosition 함수의 기능을 추가한 것입니다.
GetPosition은 bRandom 값에 의해 true이면, 랜덤 함수에 기반한 무작위의 위치 값을 제공하도록 하고, false인 경우 인자로 받은 time 값에 기반하여 원 운동을 하도록 위치값을 제공합니다.

### main.cpp 설명

#### drawRobotArm
drawRobotArm 함수가 로봇팔을 그리는 역할을 하는 함수입니다.
로봇팔을 그릴 때 유의해야하는데, 원점을 기준으로 로폿팔을 그릴 때, 팔의 길이에서 반만큼의 값의 위치에서 ang만큼 회전한 후, 받침대에 알맞은 위치로 옮기기 위해 z축 기준으로 이동을 한 후 스케일링을 주어야한다는 것을 명심해야합니다.
* 원점을 기준으로 해당 위치에서 회전한 물체가 일정 크기로 존재한다 => Scale * Rotation * Translation 연산 순서 규칙에 유의한 결과.

#### computeAngle 
computeAngle은 위에서 설명한 gradient descent를 활용한 연산을 수행하여 ang 값을 할당해주는 역할의 함수입니다.
