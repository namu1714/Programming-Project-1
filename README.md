# 컴퓨터공학 설계 및 실험 1

### 1. 테트리스
#### ncurses 라이브러리가 제공하는 리눅스 터미널 상에서의 GUI를 이용하여 테트리스 게임 프로그램을 제작하고 여기에 랭킹 시스템과 블록 배치 추천 기능을 추가 구현한다
- 메뉴 화면

![image](https://user-images.githubusercontent.com/60068586/115950173-b2a4b600-a514-11eb-833b-97e2cfbe1a10.png)

- 게임 플레이 화면 : 그림자 기능, 추천 기능, 2개의 블록 미리보기가 구현됨

![image](https://user-images.githubusercontent.com/60068586/115950191-d23bde80-a514-11eb-9d2e-fe1cb63ad46a.png)

- 랭킹 시스템

![image](https://user-images.githubusercontent.com/60068586/115950221-f39cca80-a514-11eb-9142-d51551d9b98b.png)

- 자동으로 추천된 위치에 블록이 놓아지는 추천 플레이 화면

![image](https://user-images.githubusercontent.com/60068586/115950235-0b744e80-a515-11eb-8eda-343ec1823201.png)


### 2. 미로찾기
#### 미로를 만드는 프로그램과 미로를 보여주는 GUI 프로그램, 그리고 미로에서 가장 짧은 경로를 찾는 프로그램을 제작한다
- Eller’s Algorithm을 이용해 완전 미로, 불완전 미로 생성

![미로제작](https://user-images.githubusercontent.com/60068586/115949951-52f9db00-a513-11eb-851c-978dc359e12f.jpg)

- .maz 확장자를 가지는 text 파일을 불러와 MFC 도구를 통해 미로를 그림

![미로만들기](https://user-images.githubusercontent.com/60068586/115949956-5beaac80-a513-11eb-9271-eacf4e35f6c6.jpg)

- DFS/BFS 알고리즘을 이용해 미로 탐색

![미로찾ㄱ](https://user-images.githubusercontent.com/60068586/115949965-66a54180-a513-11eb-9dfa-c38784181650.jpg)
