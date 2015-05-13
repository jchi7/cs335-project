#include "jasjotS.h" // UnitTests header

ofstream logfile;

int main()
{
  logfile.open("unit_test_results.txt");
  if (!logfile.is_open()) {
    cout << "failed to open log file\n";
    exit(1);
  }

  bool allTestsOk = true;
  bool collisionsResult, vecFxnsResult, basicEnemyResult;

  collisionsResult = testCollisions();
  vecFxnsResult = testVecFxns();
  basicEnemyResult = testBasicEnemy();
  setStickyFalse(&allTestsOk, collisionsResult);
  setStickyFalse(&allTestsOk, vecFxnsResult);
  setStickyFalse(&allTestsOk, basicEnemyResult);

  logfile << endl << endl
    << "----- ----- ----- ----- -----\n"
    << "           RESULTS           \n";
  if (allTestsOk) {
    logfile << "  ALL TESTS PASSED!\n";
  }
  else {
    logfile << "  FAILED:\n"
     << "    collisions: " << printPassFail(collisionsResult) << endl
     << "    vecFxns: " << printPassFail(vecFxnsResult) << endl
     << "    basicEnemy: " << printPassFail(basicEnemyResult) << endl;
  }
  logfile << "----- ----- ----- ----- -----\n";

  logfile.close();
  return 0;
}

string printPassFail(bool score)
{
  if (score) {return "PASSED";}
  else {return "FAILED";}
}

string printCollision(bool result)
{
  if (result) {return "COLLISION";}
  else {return "OUTSIDE";}
}

void setStickyFalse(bool * flagToSet, bool condition)
{
  if ((*flagToSet) == false) {return;}
  else {*flagToSet = condition;}
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*                    Collisions Tests                   */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- */
bool testCollisions()
{
  bool passflag = true;
  bool ptInRectFlag, threeHalfFlag;
  logfile << "\n\n---- Testing Collisions ----\n";
  ptInRectFlag = testPtInRect();
  threeHalfFlag = testThreeHalfSpace();
  setStickyFalse(&passflag, ptInRectFlag);
  setStickyFalse(&passflag, threeHalfFlag);
  logfile << "\n---- " << printPassFail(passflag) << " Collisions Test ----\n";
  if (!passflag) {
    logfile << "  Collisions Summary:\n"
      << printPassFail(ptInRectFlag) << " ptInRect\n"
      << printPassFail(threeHalfFlag) << " threeHalfSpace\n";
  }
  return passflag;
}

bool testThreeHalfSpace()
{
  bool unitResult = true;
  Shape t;
  t.type = TRIANGLE;
  vecMake(2,-1,t.corners[0]);
  vecMake(1,-3,t.corners[1]);
  vecMake(3,-3,t.corners[2]);

  Vec centroid, midpts[3], halfMidToCenter[3], outsideCorner[3], outsideSide[3];
  vecMake(
    ((float)(t.corners[0][0]+t.corners[1][0]+t.corners[2][0]))/3.0,
    ((float)(t.corners[0][1]+t.corners[1][1]+t.corners[2][1]))/3.0,
    centroid);
  for (int i = 0; i < 3; i++) {
    vecMake(
      ((float)(t.corners[(i+1)%3][0]+t.corners[(i+2)%3][0]))/2.0,
      ((float)(t.corners[(i+1)%3][1]+t.corners[(i+2)%3][1]))/2.0,
      midpts[i]);
  }
  for (int i = 0; i < 3; i++) {
    vecMake(
      ((float)(centroid[0]+midpts[i][0]))/2.0,
      ((float)(centroid[1]+midpts[i][1]))/2.0,
      halfMidToCenter[i]);
  }
  for (int i = 0; i < 3; i++) {
    vecMake(
      t.corners[i][0]+(centroid[0]-halfMidToCenter[i][0]),
      t.corners[i][1]+(centroid[1]-halfMidToCenter[i][1]),
      outsideCorner[i]);
  }
  for (int i = 0; i < 3; i++) {
    vecMake(
      midpts[i][0]+(midpts[i][0]-halfMidToCenter[i][0]),
      midpts[i][1]+(midpts[i][1]-halfMidToCenter[i][1]),
      outsideSide[i]);
  }

  logfile << "\n---- Testing threeHalfSpace ----\n"
    << "  centroid:\n";
  setStickyFalse(
    &unitResult,
    singleRunThreeHalfSpace(&t,centroid[0],centroid[1],1));
  logfile << "  corners:\n";
  for (int i = 0; i < 3; ++i) {
    setStickyFalse(
      &unitResult,
      singleRunThreeHalfSpace(&t,t.corners[i][0],t.corners[i][1],1));
  }
  logfile << "  midpoints along sides:\n";
  for (int i = 0; i < 3; ++i) {
    setStickyFalse(
      &unitResult,
      singleRunThreeHalfSpace(&t,midpts[i][0],midpts[i][1],1));
  }
  logfile << "  halfway from midpoint to center:\n";
  for (int i = 0; i < 3; ++i) {
    setStickyFalse(
      &unitResult,
      singleRunThreeHalfSpace(&t,halfMidToCenter[i][0],halfMidToCenter[i][1],1));
  }
  logfile << "  directly out from corner:\n";
  for (int i = 0; i < 3; ++i) {
    setStickyFalse(
      &unitResult,
      singleRunThreeHalfSpace(&t,outsideCorner[i][0],outsideCorner[i][1],0));
  }
  logfile << "  directly out from sides:\n";
  for (int i = 0; i < 3; ++i) {
    setStickyFalse(
      &unitResult,
      singleRunThreeHalfSpace(&t,outsideSide[i][0],outsideSide[i][1],0));
  }
  return unitResult;
}

bool singleRunThreeHalfSpace(Shape * t, float x, float y, bool answ)
{
  bool testResult, result;
  Vec pt;

  vecMake(x,y,pt);
  result = threeHalfSpace(t, pt);
  logfile << "  pt = "<< vecPrint(pt) <<",\n    ";
  for (int i = 0; i < 3; i++) {
    logfile << "t:corners["<<i<<"] = "<< vecPrint(t->corners[i]);
    if (i != 2) {
      logfile << ", ";
    }
  }
  logfile << endl
    << "    threeHalfSpace() = " << printCollision(result) << ": ";
  testResult = (answ == result);
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testPtInRect()
{
  bool unitResult = true;
  Shape r;
  r.type = RECTANGLE;
  vecMake(-3,3,r.center);
  r.width = 1;
  r.height = 1;

  logfile << "\n---- Testing ptInRect ----\n"
    << "  center:\n";
  setStickyFalse(
    &unitResult,
    singleRunPtInRect(&r,r.center[0],r.center[1],1));
  logfile << "  corners:\n";
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]-r.width,r.center[1]+r.height,1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]+r.width,r.center[1]+r.height,1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]-r.width,r.center[1]-r.height,1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]+r.width,r.center[1]-r.height,1));
  logfile << "  middle of rectangle quadrants:\n";
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]+((float)r.width/2.0),
      r.center[1]-((float)r.height/2.0),1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]+((float)r.width/2.0),
      r.center[1]+((float)r.height/2.0),1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]-((float)r.width/2.0),
      r.center[1]-((float)r.height/2.0),1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]-((float)r.width/2.0),
      r.center[1]+((float)r.height/2.0),1));
  logfile << "  along sides:\n";
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]+r.width,
      r.center[1]-((float)r.height/2.0),1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]+((float)r.width/2.0),
      r.center[1]-r.height,1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]+r.width,
      r.center[1]+((float)r.height/2.0),1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]+((float)r.width/2.0),
      r.center[1]+r.height,1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]-r.width,
      r.center[1]-((float)r.height/2.0),1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]-((float)r.width/2.0),
      r.center[1]-r.height,1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]-r.width,
      r.center[1]+((float)r.height/2.0),1));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,
      r.center[0]-((float)r.width/2.0),
      r.center[1]+r.height,1));
  logfile << " around upper left:\n";
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]-r.width,r.center[1]+r.height+1,0));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,(r.center[0]-r.width)-1,r.center[1]+r.height,0));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,(r.center[0]-r.width)-1,r.center[1]+r.height+1,0));
  logfile << "  around upper right:\n";
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]+r.width+1,r.center[1]+r.height,0));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]+r.width,r.center[1]+r.height+1,0));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]+r.width+1,r.center[1]+r.height+1,0));
  logfile << "  around lower left:\n";
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,(r.center[0]-r.width)-1,r.center[1]-r.height,0));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]-r.width,1,0));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,(r.center[0]-r.width)-1,(r.center[1]-r.height)-1,0));
  logfile << "  around lower right:\n";
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]+r.width+1,r.center[1]-r.height,0));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]+r.width,1,0));
  setStickyFalse(
    &unitResult, 
    singleRunPtInRect(&r,r.center[0]+r.width+1,(r.center[1]-r.height)-1,0));
  logfile << "  directly out from sides:\n";
  setStickyFalse(
    &unitResult,
    singleRunPtInRect(&r,(r.center[0]-r.width)-1,r.center[1],0));
  setStickyFalse(
    &unitResult,
    singleRunPtInRect(&r,r.center[0],r.center[1]+r.height+1,0));
  setStickyFalse(
    &unitResult,
    singleRunPtInRect(&r,r.center[0]+r.width+1,r.center[1],0));
  setStickyFalse(
    &unitResult,
    singleRunPtInRect(&r,r.center[0],(r.center[1]-r.height)-1,0));
  return unitResult;
}

bool singleRunPtInRect(Shape * r, float x, float y, bool answ)
{
  bool testResult, result;
  Vec pt;

  vecMake(x,y,pt);
  result = ptInRect(r, pt);
  logfile << "  pt = "<< vecPrint(pt) <<",\n"
    << "    r:center = "<< vecPrint(r->center) << ", r:width = " << r->width << ", r:height = " << r->height << endl
    << "    ptInRect() = " << printCollision(result) << ": ";
  testResult = (answ == result);
  logfile << printPassFail(testResult) << endl;
  return testResult;
}


/* ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*                     VecFxns Tests                     */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- */
bool testVecFxns()
{
  bool passflag = true;
  bool dotFlag, subFlag, lenFlag, distFlag, normFlag, ppdFlag, hXFlag, lXFlag, hYFlag, lYFlag;
  logfile << "\n\n---- Testing VecFxns ----\n";
  dotFlag = testVecDotProduct();
  subFlag = testVecSub();
  lenFlag = testVecLength();
  distFlag = testVecDist();
  normFlag = testVecNormalize();
  ppdFlag = testVecPerpendicular();
  hXFlag = testGetHighestX();
  lXFlag = testGetLowestX();
  hYFlag = testGetHighestY();
  lYFlag = testGetLowestY();
  setStickyFalse(&passflag, dotFlag);
  setStickyFalse(&passflag, subFlag);
  setStickyFalse(&passflag, lenFlag);
  setStickyFalse(&passflag, distFlag);
  setStickyFalse(&passflag, normFlag);
  setStickyFalse(&passflag, ppdFlag);
  setStickyFalse(&passflag, hXFlag);
  setStickyFalse(&passflag, lXFlag);
  setStickyFalse(&passflag, hYFlag);
  setStickyFalse(&passflag, lYFlag);
  logfile << "\n---- " << printPassFail(passflag) << " vecFxns Test ----\n";
  if (!passflag) {
    logfile << "  vecFxns Summary:\n"
      << printPassFail(dotFlag) << " vecDotProduct\n"
      << printPassFail(subFlag) << " vecSub\n"
      << printPassFail(lenFlag) << " vecLength\n"
      << printPassFail(distFlag) << " vecLength\n"
      << printPassFail(normFlag) << " vecNormalize\n"
      << printPassFail(ppdFlag) << " vecPerpendicular\n"
      << printPassFail(hXFlag) << " getHighestX\n"
      << printPassFail(lXFlag) << " getLowestX\n"
      << printPassFail(hYFlag) << " getHighestY\n"
      << printPassFail(lYFlag) << " getLowestY\n";
  }
  return passflag;
}

bool testVecDotProduct()
{
  bool unitResult = true;
  logfile << "\n---- Testing vecDotProduct ----\n";
  setStickyFalse(&unitResult, singleRunVecDot(0,0,0,0,0));
  setStickyFalse(&unitResult, singleRunVecDot(2,3,-2,-3,-13));
  setStickyFalse(&unitResult, singleRunVecDot(4,-3,6,5,9));
  return unitResult;
}

bool singleRunVecDot(float x1, float y1, float x2, float y2, float expected)
{
  bool testResult;
  Vec pt1, pt2;
  float result, eps = 0.1;

  vecMake(x1,y1,pt1);
  vecMake(x2,y2,pt2);
  result = vecDotProduct(pt1,pt2);
  logfile << "  pt1 = "<< vecPrint(pt1) <<", pt2 = "<< vecPrint(pt2) << endl
    << "    pt1 . pt2 = " << result << ": ";
  testResult = abs(result - expected) < eps;
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testVecSub()
{
  bool unitResult = true;
  logfile << "\n---- Testing vecSub(To,From,Result) ----\n";
  setStickyFalse(&unitResult, singleRunVecSub(0,0,-3,4,3,-4));
  setStickyFalse(&unitResult, singleRunVecSub(6,7,-3,4,9,3));
  return unitResult;
}

bool singleRunVecSub(float x1, float y1, float x2, float y2, float answX, float answY)
{
  bool testResult;
  const float eps = 0.1;
  Vec pt1, pt2, pt3;

  vecMake(x1,y1,pt1);
  vecMake(x2,y2,pt2);
  vecSub(pt1,pt2,pt3);
  logfile << "  pt1 = "<< vecPrint(pt1) <<", pt2 = "<< vecPrint(pt2) << endl
    << "    pt1 - pt2 = " << vecPrint(pt3) << ": ";
  testResult = ((abs(pt3[0] - answX) < eps) && (abs(pt3[1] - answY) < eps));
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testVecLength()
{
  bool unitResult = true;
  logfile << "\n---- Testing vecLength ----\n";
  setStickyFalse(&unitResult, singleRunVecLen(0,0,0));
  setStickyFalse(&unitResult, singleRunVecLen(3,4,5));
  setStickyFalse(&unitResult, singleRunVecLen(-2,0,2));
  setStickyFalse(&unitResult, singleRunVecLen(0,5,5));
  setStickyFalse(&unitResult, singleRunVecLen(-5,3,5.8310));
  return unitResult;
}

bool singleRunVecLen(float x, float y, float answ)
{
  bool testResult;
  float result;
  const float eps = 0.1;
  Vec pt;

  vecMake(x,y,pt);
  result = vecLength(pt);
  logfile << "  pt = " << vecPrint(pt) << ", length = " << result << ": ";
  testResult = abs(result - answ) < eps;
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testVecDist()
{
  bool unitResult = true;
  logfile << "\n---- Testing vecDist(pt1,pt2) ----\n";
  setStickyFalse(&unitResult, singleRunVecDist(1,2,1,2,0));
  setStickyFalse(&unitResult, singleRunVecDist(0,0,3,4,5));
  setStickyFalse(&unitResult, singleRunVecDist(2,4,3,-5,9.0554));
  setStickyFalse(&unitResult, singleRunVecDist(-8,-2,-4,4,7.2111));
  return unitResult;
}

bool singleRunVecDist(float x1, float y1, float x2, float y2, float answ)
{
  bool testResult;
  float result;
  const float eps = 0.1;
  Vec pt1, pt2;

  vecMake(x1,y1,pt1);
  vecMake(x2,y2,pt2);
  result = vecDist(pt1,pt2);
  logfile << "  pt1 = "<< vecPrint(pt1) <<", pt2 = "<< vecPrint(pt2) << endl
    << "    vecDist(pt1,pt2) = " << result << ": ";
  testResult = abs(result - answ) < eps;
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testVecNormalize()
{
  bool unitResult = true;
  logfile << "\n---- Testing vecNormalize ----\n";
  // to double check: divide x&y each by vector length
  setStickyFalse(&unitResult, singleRunVecNorm(0,0,0,0));
  setStickyFalse(&unitResult, singleRunVecNorm(2,0,1,0));
  setStickyFalse(&unitResult, singleRunVecNorm(0,-3,0,-1));
  setStickyFalse(&unitResult, singleRunVecNorm(5,-6,0.6402,-0.7682));
  setStickyFalse(&unitResult, singleRunVecNorm(1,2,0.4472,0.8944));
  setStickyFalse(&unitResult, singleRunVecNorm(-3,4,-0.6,0.8));
  setStickyFalse(&unitResult, singleRunVecNorm(-2,-4,-0.4472,-0.8944));
  return unitResult;
}

bool singleRunVecNorm(float x1, float y1, float answX, float answY)
{
  bool testResult;
  const float eps = 0.001;
  Vec pt;

  vecMake(x1,y1,pt);
  logfile << "  pt = " << vecPrint(pt) << ", normalized = ";
  vecNormalize(pt);
  logfile << vecPrint(pt) << ": ";
  testResult = ((abs(pt[0] - answX) < eps) && (abs(pt[1] - answY) < eps));
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testVecPerpendicular()
{
  bool unitResult = true;
  logfile << "\n---- Testing vecPerpendicular(source,dest) ----\n";
  // assuming function get perpendicular moving counter-clockwise
  setStickyFalse(&unitResult, singleRunVecPpd(0,0,-0,0));
  setStickyFalse(&unitResult, singleRunVecPpd(-3,4,-4,-3));
  setStickyFalse(&unitResult, singleRunVecPpd(2,0,-0,2));
  setStickyFalse(&unitResult, singleRunVecPpd(0,5,-5,0));
  setStickyFalse(&unitResult, singleRunVecPpd(2,-7,7,2));
  setStickyFalse(&unitResult, singleRunVecPpd(-5,-3,3,-5));
  setStickyFalse(&unitResult, singleRunVecPpd(1,6,-6,1));
  return unitResult;
}

bool singleRunVecPpd(float x, float y, float answX, float answY)
{
  bool testResult;
  const float eps = 0.1;
  Vec pt, ppd;

  vecMake(x,y,pt);
  vecPerpendicular(pt,ppd);
  logfile << "  pt = " << vecPrint(pt) << ", perpendicular = " << vecPrint(ppd) << ": ";
  testResult = ((abs(ppd[0] - answX) < eps) && (abs(ppd[1] - answY) < eps));
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testGetHighestX()
{
  bool unitResult = true;
  logfile << "\n---- Testing GetHighestX ----\n";
  setStickyFalse(&unitResult, singleRunGetHighestX(1,2,1,2,1,2,1));
  setStickyFalse(&unitResult, singleRunGetHighestX(-1,2,-2,3,-3,4,-1));
  setStickyFalse(&unitResult, singleRunGetHighestX(1.2,0,1.1,-1,1,-1.1,1.2));
  setStickyFalse(&unitResult, singleRunGetHighestX(2,1,5.1,3.3,-1,-2.1,5.1));
  setStickyFalse(&unitResult, singleRunGetHighestX(0,0,-1.2,-2.1,3.2,4.1,3.2));
  return unitResult;
}

bool singleRunGetHighestX(float x1, float y1, float x2, float y2, float x3, float y3, float answ)
{
  bool testResult;
  float result;
  const float eps = 0.001;
  Vec pt1, pt2, pt3;

  vecMake(x1,y1,pt1); vecMake(x2,y2,pt2); vecMake(x3,y3,pt3);
  result = getHighestX(pt1,pt2,pt3);
  logfile << "  pt1 = " << vecPrint(pt1) << ", pt2 = " << vecPrint(pt2) << ", pt3 = " << vecPrint(pt3) << endl
    << "    highest x value = " << result << ": ";
  testResult = (abs(result - answ) < eps);
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testGetLowestX()
{
  bool unitResult = true;
  logfile << "\n---- Testing GetLowestX ----\n";
  setStickyFalse(&unitResult, singleRunGetLowestX(1,2,1,2,1,2,1));
  setStickyFalse(&unitResult, singleRunGetLowestX(-1,2,-2,3,-3,4,-3));
  setStickyFalse(&unitResult, singleRunGetLowestX(1.2,0,1.1,-1,1,-1.1,1));
  setStickyFalse(&unitResult, singleRunGetLowestX(2,1,5.1,3.3,-1,-2.1,-1));
  setStickyFalse(&unitResult, singleRunGetLowestX(0,0,-1.2,-2.1,3.2,4.1,-1.2));
  return unitResult;
}

bool singleRunGetLowestX(float x1, float y1, float x2, float y2, float x3, float y3, float answ)
{
  bool testResult;
  float result;
  const float eps = 0.001;
  Vec pt1, pt2, pt3;

  vecMake(x1,y1,pt1); vecMake(x2,y2,pt2); vecMake(x3,y3,pt3);
  result = getLowestX(pt1,pt2,pt3);
  logfile << "  pt1 = " << vecPrint(pt1) << ", pt2 = " << vecPrint(pt2) << ", pt3 = " << vecPrint(pt3) << endl
    << "    lowest x value = " << result << ": ";
  testResult = (abs(result - answ) < eps);
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testGetHighestY()
{
  bool unitResult = true;
  logfile << "\n---- Testing GetHighestY ----\n";
  setStickyFalse(&unitResult, singleRunGetHighestY(1,2,1,2,1,2,2));
  setStickyFalse(&unitResult, singleRunGetHighestY(-1,2,-2,3,-3,4,4));
  setStickyFalse(&unitResult, singleRunGetHighestY(1.2,0,1.1,-1,1,-1.1,0));
  setStickyFalse(&unitResult, singleRunGetHighestY(2,1,5.1,3.3,-1,-2.1,3.3));
  setStickyFalse(&unitResult, singleRunGetHighestY(0,0,-1.2,-2.1,3.2,4.1,4.1));
  return unitResult;
}

bool singleRunGetHighestY(float x1, float y1, float x2, float y2, float x3, float y3, float answ)
{
  bool testResult;
  float result;
  const float eps = 0.001;
  Vec pt1, pt2, pt3;

  vecMake(x1,y1,pt1); vecMake(x2,y2,pt2); vecMake(x3,y3,pt3);
  result = getHighestY(pt1,pt2,pt3);
  logfile << "  pt1 = " << vecPrint(pt1) << ", pt2 = " << vecPrint(pt2) << ", pt3 = " << vecPrint(pt3) << endl
    << "    highest y value = " << result << ": ";
  testResult = (abs(result - answ) < eps);
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

bool testGetLowestY()
{
  bool unitResult = true;
  logfile << "\n---- Testing GetLowestY ----\n";
  setStickyFalse(&unitResult, singleRunGetLowestY(1,2,1,2,1,2,2));
  setStickyFalse(&unitResult, singleRunGetLowestY(-1,2,-2,3,-3,4,2));
  setStickyFalse(&unitResult, singleRunGetLowestY(1.2,0,1.1,-1,1,-1.1,-1.1));
  setStickyFalse(&unitResult, singleRunGetLowestY(2,1,5.1,3.3,-1,-2.1,-2.1));
  setStickyFalse(&unitResult, singleRunGetLowestY(0,0,-1.2,-2.1,3.2,4.1,-2.1));
  return unitResult;
}

bool singleRunGetLowestY(float x1, float y1, float x2, float y2, float x3, float y3, float answ)
{
  bool testResult;
  float result;
  const float eps = 0.001;
  Vec pt1, pt2, pt3;

  vecMake(x1,y1,pt1); vecMake(x2,y2,pt2); vecMake(x3,y3,pt3);
  result = getLowestY(pt1,pt2,pt3);
  logfile << "  pt1 = " << vecPrint(pt1) << ", pt2 = " << vecPrint(pt2) << ", pt3 = " << vecPrint(pt3) << endl
    << "    lowest y value = " << result << ": ";
  testResult = (abs(result - answ) < eps);
  logfile << printPassFail(testResult) << endl;
  return testResult;
}

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/*                      Class Tests                      */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- */
bool testBasicEnemy()
{
  BasicEnemy testEnemy(200, 300, 100, 250);
  logfile << "\n\n---- Testing Class: Basic Enemy ----\n"
    << "  Declaring: testEnemy(200, 300, 100, 250)\n"
    << "  Results:\n" << testEnemy.debugReport() << endl;
  return true; // !!!! need to parse string and check results !!!!
}

