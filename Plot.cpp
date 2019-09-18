#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVariantArray.h>
#include <vtkNew.h>
#include <vtkVersion.h>
#include <vtkAxis.h>

#include <iostream>
#include <vector>
#include <thread>
#include <string>

using namespace std;

void printVector(const string &name, const vector<float> &vector);

int main(int argc, char **argv)
{
  cout << "Using: " << vtkVersion::GetVTKSourceVersion() << '\n';

  // SHIFT will be added to the X coordinates of the plots.
  const float SHIFT = (argc < 2) ? 0.0f : stof(argv[1]);

  // Declare vtkContextView and a vtkChartXY to its scene.
  vtkNew<vtkContextView> view;
  view->GetRenderWindow()->SetSize(1920, 1080);

  vtkNew<vtkChartXY> chart;
  view->GetScene()->AddItem(chart);

  // Fix the range on y axis to better show the issue effect.
  // Range is fixed in such a way that the following plots are well between its boundaries.
  chart->GetAxis(vtkAxis::LEFT)->SetRange(0.0, 3.0);
  chart->GetAxis(vtkAxis::LEFT)->SetBehavior(vtkAxis::FIXED);

  // Build the data table from std::vector<float>.
  // For this example there are 5 data points.
  vtkNew<vtkTable> table;

  vtkNew<vtkFloatArray> arrX;
  arrX->SetName("X Axis");
  table->AddColumn(arrX);

  vtkNew<vtkFloatArray> arrY;
  arrY->SetName("Y Axis - POINTS");
  table->AddColumn(arrY);

  vtkNew<vtkFloatArray> arrL;
  arrL->SetName("Y Axis - LINE");
  table->AddColumn(arrL);

  // Set the data on the different vtkFloatArrays from vector<float> objects.
  vector<float> x = {SHIFT + 1.0f, SHIFT + 2.0f, SHIFT + 3.0f, SHIFT + 4.0f, SHIFT + 5.0f};
  arrX->SetArray(x.data(), x.size(), 1);
  printVector("x_data", x);

  vector<float> y(5, 1.0f);
  arrY->SetArray(y.data(), y.size(), 1);
  printVector("y_data_point_plot", x);

  vector<float> y2(5, 2.0f);
  arrL->SetArray(y2.data(), y2.size(), 1);
  printVector("y_data_line_plot", x);

  auto plot = chart->AddPlot(vtkChart::POINTS);

  plot->SetInputData(table, 0, 1);
  plot->SetColor(255, 0, 0, 255);
  plot->SetWidth(5.0);

  auto line = chart->AddPlot(vtkChart::LINE);

  line->SetInputData(table, 0, 2);
  line->SetColor(0, 0, 255, 255);
  line->SetWidth(5.0);

  view->Render();
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  return 0;
}

void printVector(const string &name, const vector<float> &vector)
{
  cout << name << " = {";

  for (auto &&val : vector)
    cout << val << ", ";

  cout << "\b\b}" << endl;
}
