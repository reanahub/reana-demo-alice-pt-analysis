# Tests to ensure the data is correctly processed

Feature: Correct processing of data

  Precise data processing within our workflow is paramount to deliver
  trustworthy research analysis results.
  Test that the data is properly elaborated in the workflow.

  Scenario: The data is fetched from the correct remote location
    When the workflow execution completes
    Then the engine logs should contain "curl -fsS --retry 9 -o ./data/AliESDs.root http://opendata.cern.ch/eos/opendata/alice/2010/LHC10h/000139038/ESD/0003/AliESDs.root"
    And the workspace should include "data/AliESDs.root"

  Scenario: The size of the processed data stays within reasonable limits
    When the workflow execution completes
    Then the size of the file "data/AliESDs.root" should be between 300 MiB and 400 MiB

  Scenario: The data processing steps are correctly logged
    When the workflow execution completes
    Then the logs should contain "Processing ./runEx01.C..."
    And the workspace should include "run.log"
    And the file "run.log" should contain "Processing ./runEx01.C..."


