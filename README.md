# vtkScatterPlotBug

#### Introduction

This repository contains an example program that reproduces a bug in the VTK library related to 2D plotting.

The version used (`v8.2.0`) is the lastest available in the [official VTK repository](`https://gitlab.kitware.com/vtk/vtk`).

The issue was found by modifying the [scatter plot test shown in the VTK repository](https://gitlab.kitware.com/vtk/vtk/blob/master/Charts/Core/Testing/Cxx/TestScatterPlot.cxx).

#### Description

When adding a `vtkPlot` to a `vtkChartXY` object, its content is not rendered correctly if:

1. `vtkRenderWindowInteractor` is not being used and the view is rendered directly by the user (situation common in off-screen rendering usage).
2. The `x` data _values_ are ~100 times the `x` data _range_ (more details in the Example section).

The provided example shows this behavior both for `vtkChart::LINE` and `vtkChart::POINTS` plots.

Y-axis range is fixed between `0.0` to `3.0` to better show the issue effect. Range is fixed in such a way that the plots are well between its boundaries.

Once compiled the example program can be used with:

```
Use: ./Plot [X DATA SHIFT] [ENABLE WORKAROUND]
Where:
[X DATA SHIFT]: floating point constant that will be added to all X coordinates.
[ENABLE WORKAROUND]: integer(0,1) enables or disables the workaround.
```

#### Examples

The example in this repository shows how it's possible to reproduce the issue for a fixed data range of:

`x_range = x_max - m_min = 4`

```
$ ./Plot 0.0 0
Using: vtk version 8.2.0
x_data = {1, 2, 3, 4, 5} 
y_data_point_plot = {1, 1, 1, 1, 1} 
y_data_line_plot = {2, 2, 2, 2, 2}
```

A chart is correctly shown as seen in the picture:

![Correct chart](https://github.com/marcoreato/vtkScatterPlotBug/blob/master/correct_plot.jpeg)

By shifting the ordinates of `400 = x_range * 100`:

```
$ ./Plot 400 0
Using: vtk version 8.2.0
x_data = {401, 402, 403, 404, 405} 
y_data_point_plot = {1, 1, 1, 1, 1} 
y_data_line_plot = {2, 2, 2, 2, 2}}
```

The chart is not correctly displayed, as seen in the picture: 

![Wrong chart](https://github.com/marcoreato/vtkScatterPlotBug/blob/master/empty_plot.jpeg)

While the scale of the axis is correct the two `vtkPlot` are not shown.

#### Workaround

In the previous examples the view was rendered only once. An empirical workaround to this issue consists in marking the view as `Modified()` and re-render it a second time. The code snippet is the following:

```
  view->Render();

  if (WORKAROUNT_ENABLED)
  {
    view->Modified();
    view->Render();
  }
```

By running the second example with the workaround activated the results are:

```
Using: vtk version 8.2.0
WORKAROUNT ENABLED!
x_data = {401, 402, 403, 404, 405} 
y_data_point_plot = {1, 1, 1, 1, 1} 
y_data_line_plot = {2, 2, 2, 2, 2} 
```

The plot is correctly rendered as seen in the picture:

![Workaround chart](https://github.com/marcoreato/vtkScatterPlotBug/blob/master/workaround_plot.jpeg)