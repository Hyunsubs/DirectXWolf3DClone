빌드 결과는 OutputFile/bin에 생성 되도록 하였음
fmod와 fw1fontwrapper의 DLL 파일이 빌드된 실행파일에 같이 들어있어야 함
비주얼 스튜디오에서 f5로 실행하면 Project 폴더를 기준으로 프로그램이 실행되기 때문에 Content찾는데 오류가 생겨
OutputFile이 실행되도록 설정을 바꿔주어야 함(프로젝트 설정->디버깅->Working Directory를 ($SolutionDir)\\OutpuFile\\bin 으로 변경할것)
