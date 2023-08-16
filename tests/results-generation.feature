# Tests to ensure the results are generated correctly

Feature: Reliable results generation

  Ensure that every result produced is correctly generated
  and is immediately visible and accessible within the workspace.

  Scenario: The workflow successfully finishes
    When the workflow execution completes
    Then the workflow status should be finished

  Scenario: The expected result ROOT file is present
    When the workflow is finished
    Then the workspace should include "AnalysisResults.root"
    And all the outputs should be included in the workspace

  Scenario: The expected PDF plots are generated
    When the workflow is finished
    Then the workspace should include "plot.log"
    And the file "plot.log" should contain "Processing ./plot.C..."
    And the logs should contain "Processing ./plot.C..."
    And the workspace should include "plot_eta.pdf"
    And the workspace should include "plot_pt.pdf"
    And the logs should contain "pdf file plot_pt.pdf has been created"
    And the logs should contain "pdf file plot_eta.pdf has been created"

  Scenario: After finishing, the size of the workspace stays within reasonable limits
    When the workflow is finished
    Then the workspace size should be more than 150 MiB
    And the workspace size should be less than 400 MiB
