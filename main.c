#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARRAY_COUNT(array) (int)(sizeof(array)/sizeof((array)[0]))
#define ARRAY_LAST(array) (array)[ARRAY_COUNT(array)-1]
#define FOR_HELPER(array, count) for (int i = 0; i < count;) for (__typeof__((array)[0]) it = {0}; memcpy(it, (array)[i], sizeof(it)) && i < count; ++i)
#define FOR_ARRAY(array) FOR_HELPER(array, ARRAY_COUNT(array))
#define FOR(array) FOR_HELPER(array, array##Count)

#define UNUSED(x) (void)(x)

float rand01() { return (float)(rand() / (double)RAND_MAX); }
float square(float x) { return x*x; }

typedef struct {
  float w;
  float b;
} Model;

float modelPredict(Model m, float x) {
  return m.w*x+m.b;
}

// average squared distance between predicted and actual y
float modelLoss(Model m, float data[][2], int dataCount) {
  float loss = 0.0;
  FOR (data) {
    float x = it[0];
    float y = it[1];
    float predictedY = modelPredict(m, x);
    loss += square(predictedY - y);
  }
  return loss / dataCount;
}

int main() {
  float twiceData[][2] = {
    { 0.0, 0.0 },
    { 1.0, 2.0 },
    { 2.0, 4.0 },
    { 3.0, 6.0 },
    { 4.0, 8.0 },
  };

  float squareData[][2] = {
    { 0.0,  0.0 },
    { 1.0,  1.0 },
    { 2.0,  4.0 },
    { 3.0,  9.0 },
    { 4.0, 16.0 },
  };
  UNUSED(squareData);

  int seed = time(0);
  srand(seed);
  printf("seed=%d\n", seed);
  printf("----------------------------------\n");

  float eps = 1e-3;
  float rate = 100.0;

  Model m = {0};
  m.w = rand01();

  printf("eps=%.3f, rate=%.3f\n", eps, rate);
  printf("----------------------------------\n");

  float oldLoss = 9999.999;
  float loss = 999.999;
  int i = 0;
  while (loss > eps && i < 1'000'000 && (oldLoss-loss) > eps) {
    printf("%03d | loss=%.3f, w=%.3f, b=%.3f\n", i, loss, m.w, m.b);

    oldLoss = loss;
    loss = modelLoss(m, twiceData, ARRAY_COUNT(twiceData));

    Model mDw = m;
    mDw.w += eps;
    float lossDw = modelLoss(mDw, twiceData, ARRAY_COUNT(twiceData)) - loss;

    Model mDb = m;
    mDb.b += eps;
    float lossDb = modelLoss(mDb, twiceData, ARRAY_COUNT(twiceData)) - loss;

    m.w -= rate * lossDw;
    m.b -= rate * lossDb;
    
    ++i;
  }

  printf("----------------------------------\n");
  printf("end | loss=%.3f, w=%.3f, b=%.3f\n", loss, m.w, m.b);
  printf("----------------------------------\n");
  FOR_ARRAY (twiceData) {
    float x = it[0];
    float y = it[1];
    printf("%03d | predict(%.3f) = %.3f (actual=%.3f)\n", i, x, modelPredict(m, x), y);
  }
}
