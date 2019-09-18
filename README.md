# vtkScatterPlotBug

#### Introduction

This repository contains an example program that reproduces a bug in the VTK library related to 2D plotting.

The version used (`v8.2.0`) is the lastest available in the [official VTK repository](`https://gitlab.kitware.com/vtk/vtk`).

The issue was found by modifying the [scatter plot test shown in the VTK repository](https://gitlab.kitware.com/vtk/vtk/blob/master/Charts/Core/Testing/Cxx/TestScatterPlot.cxx).

#### Description

When adding a `vtkPlot` to a `vtkChartXY` object, its content is not rendered correctly if:

1. `vtkRenderWindowInteractor` is not being used and the view is rendered directly by the user (situation common in off-screen rendering usage).
2. The `x` data _values_ are ~100 times the `x` data _range_ (more details in the Example section).
3. The provided example shows this behavior both for `vtkChart::LINE` and `vtkChart::POINTS` plots.

#### Example

The example in this repository shows how it's possible to reproduce the issue for a fixed data range of:

`x_range = x_max - m_min = 4`

```
$ ./Plot 0.0
Using: vtk version 8.2.0
x_data = {1, 2, 3, 4, 5} 
y_data_point_plot = {1, 2, 3, 4, 5} 
y_data_line_plot = {1, 2, 3, 4, 5}
```

A chart is correctly shown as seen in the picture:

![Correct chart](https://github.com/marcoreato/vtkScatterPlotBug/blob/master/correct_plot.jpeg)

By shifting the ordinates of `400 = x_range * 100`:

```
$ ./Plot 400
Using: vtk version 8.2.0
x_data = {401, 402, 403, 404, 405} 
y_data_point_plot = {401, 402, 403, 404, 405} 
y_data_line_plot = {401, 402, 403, 404, 405}
```

The chart is not correctly displayed, as seen in the picture: 

![Wrong chart](https://github.com/marcoreato/vtkScatterPlotBug/blob/master/empty_plot.jpeg)

While the scale of the axis is correctly placed the two `vtkPlot` are not shown.
