====================================================
 REANA example - ALICE simple analysis demonstrator
====================================================

.. image:: https://img.shields.io/travis/reanahub/reana-demo-alice-pt-analysis.svg
   :target: https://travis-ci.org/reanahub/reana-demo-alice-pt-analysis

.. image:: https://badges.gitter.im/Join%20Chat.svg
   :target: https://gitter.im/reanahub/reana?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge

.. image:: https://img.shields.io/github/license/reanahub/reana-demo-alice-pt-analysis.svg
   :target: https://raw.githubusercontent.com/reanahub/reana-demo-alice-pt-analysis/master/LICENSE

.. image:: https://www.reana.io/static/img/badges/launch-on-reana-at-cern.svg
   :target: https://reana.cern.ch/launch?url=https%3A%2F%2Fgithub.com%2Freanahub%2Freana-demo-alice-pt-analysis&name=reana-demo-alice-pt-analysis

About
=====

This REANA reproducible analysis example demonstrates the use of the ALICE analysis framework to facilitate the analysis
of ALICE collision and simulated data samples for both pp and PbPb data taken in the year 2010.  The example was taken
from an `analysis example <http://opendata.cern.ch/record/1200>`_ from the `CERN Open Data portal <http://opendata.cern.ch/>`_
and illustrates how to convert it into a reusable analysis (`reana`) example, as it uses a complex computing environment
that is not publicly available.

ALICE physicists use custom tools that facilitate the analysis of real or simulated ALICE data
samples. This example demonstrates the use of the ALICE analysis framework on small samples of
both pp and PbPb data taken in the year 2010.

Analysis structure
==================

Making a research data analysis reproducible basically means to provide
"runnable recipes" addressing (1) where is the input data, (2) what software was
used to analyse the data, (3) which computing environments were used to run the
software and (4) which computational workflow steps were taken to run the
analysis. This will permit to instantiate the analysis on the computational
cloud and run the analysis to obtain (5) output results.

1. Input data
-------------

The analysis uses (i) an ALICE ESD data file for either pp or PbPb collisions and (ii) an ODCB magnet configuration database.
For the ESD data file, let us take an example that was published by the ALICE collaboration on the `CERN Open
Data portal <http://opendata.cern.ch/>`_ from Pb-Pb collisions. Here, we use the sample at 3.5 TeV from run number 139038 in RunH 2010.
We select the third (0003) file that is about 360 MB large in order to have a reasonable number of events for the example.

.. code-block:: console

   $ mkdir -p data
   $ cd data
   $ curl -O http://opendata.cern.ch/eos/opendata/alice/2010/LHC10h/000139038/ESD/0003/AliESDs.root
   $ cd ..

Note that ``data.txt`` file should contain the path to the downloaded sample
data file.

For the magnet configuration database, let us take the ``OCDB.root`` file released
on the `CERN Open Data portal <http://opendata.cern.ch/record/1200>`_

2. Analysis code
----------------

The user analysis is represented by a C++ class, which has to implement a few predefined methods
to process one interaction event. A template analysis class extracting the inclusive transverse
momentum and pseudorapidity spectra of all tracks can be modified to create your own analysis.

This example uses the `AliPhysics <https://github.com/alisw/AliPhysics>`_
analysis framework with the following source code files:

- `fix-env.sh <fix-env.sh>`_ - to fix the configuration environment variables
- `runEx01.C <runEx01.C>`_ - the main analysis script
- `plot.C <plot.C>`_ - script to extract data and plot the figures from the resulting ROOT file
- `AliAnalysisTaskEx01.cxx <AliAnalysisTaskEx01.cxx>`_ - the example script analysing the Pt spectrum
- `AliAnalysisTaskEx01.h <AliAnalysisTaskEx01.h>`_ - ROOT library for customization

3. Compute environment
----------------------

This example uses `AliPhysics <https://github.com/alisw/AliPhysics>`_ analysis
framework. It has been containerised as `reana-env-aliphysics
<https://github.com/reanahub/reana-env-aliphysics>`_ environment. You can fetch
some wanted AliPhysics version from Docker Hub:

.. code-block:: console

   $ docker pull docker.io/reanahub/reana-env-aliphysics:vAN-20180614-1

We shall use the ``vAN-20180614-1`` version for the present example.

Note that if you would like to build a different AliPhysics version on your own,
you can follow `reana-env-aliphysics
<https://github.com/reanahub/reana-env-aliphysics>`_ procedures and set
``ALIPHYSICS_VERSION`` environment variable appropriately:

.. code-block:: console

   $ cd src/reana-env-aliphysics
   $ export ALIPHYSICS_VERSION=vAN-20180521-1
   $ make build

4. Analysis workflow
--------------------

This analysis example consists of a script to run the task using aliroot, and then plots the results.

The computational workflow is essentially sequential in nature. We can
use the REANA serial workflow engine and represent the analysis workflow as
follows:

.. code-block:: console

                    START
                      |
                      |
                      V
   +-----------------------------------------+
   | (0) use data or download ESD input file |
   |                                         |
   |  $ curl -O http://opendata.cern.ch/...  |
   +-----------------------------------------+
                      |
                      | ALIESD.root
                      V
   +-----------------------------------------+
   | (1) perform PT analysis                 |
   |                                         |
   |    $ root -b -q './runEx01.C'           |
   +-----------------------------------------+
                      |
                      | run.logs
                      | AnalysisResults.root
                      |
                      V
   +----------------------------------------+
   | (2) plot sample histogram              |
   |                                        |
   |    $ root -b -q plot.C                 |
   +----------------------------------------+
                      |
                      | plot_pt.pdf
                      | plot_eta.pdf
                      |
                      V
                     STOP

We shall see below how this sequence of commands is represented for the REANA
serial workflow engine.

5. Output results
-----------------

The test run will create `ROOT <https://root.cern.ch/>`_ output files that
usually contain histograms.

.. code-block:: console

   $ ls -l AnalysisResults.root
   -rw-r--r-- 1 root root  31187 July 18 17:35 AnalysisResults.root

The user typically uses the output files to produce final plots. For example,
running ``plot.C`` output macro on the ``AnalysisResults.root`` output file will
permit to visualise the pt distribution of the accepted events:

.. figure:: https://raw.githubusercontent.com/reanahub/reana-demo-alice-pt-analysis/master/docs/plot_pt.png
   :alt: plot_pt.png
   :align: center

.. figure:: https://raw.githubusercontent.com/reanahub/reana-demo-alice-pt-analysis/master/docs/plot_eta.png
   :alt: plot_eta.png
   :align: center

Running the example on REANA cloud
==================================

There are two ways to execute this analysis example on REANA.

If you would like to simply launch this analysis example on the REANA instance
at CERN and inspect its results using the web interface, please click on
the following badge:

.. image:: https://www.reana.io/static/img/badges/launch-on-reana-at-cern.svg
   :target: https://reana.cern.ch/launch?url=https%3A%2F%2Fgithub.com%2Freanahub%2Freana-demo-alice-pt-analysis&name=reana-demo-alice-pt-analysis

|

If you would like a step-by-step guide on how to use the REANA command-line
client to launch this analysis example, please read on.

We start by creating a `reana.yaml <reana.yaml>`_ file describing the above
analysis structure with its inputs, code, runtime environment, computational
workflow steps and expected outputs:

.. code-block:: yaml

    version: 0.3.0
    inputs:
      files:
        - fix-env.sh
        - data.txt
        - AliAnalysisTaskEx01.cxx
        - AliAnalysisTaskEx01.h
        - runEx01.C
        - plot.C
        - OCDB.root
      parameters:
        data_location: http://opendata.cern.ch/eos/opendata/alice/2010/LHC10h/000139038/ESD/0003/AliESDs.root
    workflow:
      type: serial
      specification:
        steps:
          - environment: 'docker.io/reanahub/reana-env-aliphysics:vAN-20180614-1'
            commands:
            - mkdir -p ./data && curl -fsS --retry 9 -o ./data/AliESDs.root ${data_location}
            - source fix-env.sh && root -b -q './runEx01.C' | tee run.log
            - mkdir -p results && source fix-env.sh && root -b -q './plot.C'
    outputs:
      files:
        - AnalysisResults.root
        - results/plot_pt.pdf
        - results/plot_eta.pdf

We can now install the REANA command-line client, run the analysis and download the resulting plots:

.. code-block:: console

    $ # create new virtual environment
    $ virtualenv ~/.virtualenvs/myreana
    $ source ~/.virtualenvs/myreana/bin/activate
    $ # install REANA client
    $ pip install reana-client
    $ # connect to some REANA cloud instance
    $ export REANA_SERVER_URL=https://reana.cern.ch/
    $ export REANA_ACCESS_TOKEN=XXXXXXX
    $ # create new workflow
    $ reana-client create -n my-analysis
    $ export REANA_WORKON=my-analysis
    $ # upload input code and data to the workspace
    $ reana-client upload
    $ # start computational workflow
    $ reana-client start
    $ # ... should be finished in about 1 minute
    $ reana-client status
    $ # list output files
    $ reana-client ls | grep ".pdf"
    $ # download results root file and generated plots
    $ reana-client download

Please see the `REANA-Client <https://reana-client.readthedocs.io/>`_
documentation for more detailed explanation of typical ``reana-client`` usage
scenarios.
