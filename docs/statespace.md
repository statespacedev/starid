<img src="docs/images2/pf2-small.png" alt=""/>

20240317 switch back to github from gitlab is looking complete - github was first, 2016 to 2018 - switch to gitlab was about account troubles and comparing capabilities, 2018 to 2024 - switch back to github seems to effectively be about 'going default'. don't feel like thinking about it - go with default. with commit email address updated, and default project website activated, looks like 'third era' is rolling from today.

20230109 cleanup of docsascode - moving version up to 1.6.5.

20221228 bringing various environmental factors as far forward as possible within the github context - docs publishing via github actions to github pages, package publishing to pypi via github actions.

20221221 second thoughts about about cpp python extension here - let's leave that for project starid. in practical terms, project statespace is pure python and pycharm, project starid is cpp python extension and clion.

20220327 switch to container build environment and deploying to docker hub as well as pypi.

20210418 returning to some of the business / economic / social models orbiting around [state space time series analysis](http://ssfpack.com/DKbook.html). brought in the data files for the durbin koopman and commandeur koopman books. earlier work on the nile model brought somewhat up to date. ideally we'd like state space time series models to fit organically into the new model structure so it encompasses both business models and engineering models.

20210410 classical vector-matrix notation is go pretty much everywhere. why numpy makes this such a pain remains a mystery. during all this reconstruction, brought in [resampling from filterpy](https://filterpy.readthedocs.io/en/latest/monte_carlo/resampling.html). good stuff. [rlabbe's work](https://github.com/rlabbe/Kalman-and-Bayesian-Filters-in-Python/blob/master/12-Particle-Filters.ipynb) is highly recommended.

20210404 new model structure is working well - thoroughly checked out for kalman processor and now propagating to sigmapoint and particle processors. bearings-only model is go.

<img src="docs/images2/bearingsonly.png" alt=""/>

20210328 new model structure is definitely on the right path - it's already quite essential for working on the new bearings-only tracking model. evaluation and plotting is migrating into a new eval class within the base model. there each model will know how to make its own appropriate plots - definitely including model states and processor states - and perform its own evaluation tests. 

20210321 settling on a new model structure that separates the parts associated with baseline kalman filter, sigma-point filter, and particle filter. this can eventually grow upward into a high level base model. concept is that a model needs to know something about the processors that are going to use it. a particle processor has different needs than a kalman processor. this gives a natural shape and structure to the models.

20210314 maybe what we need is to bring in a new reference model - to kind of shake things up and help brainstorming around the concept of a high level base model. a strong candidate is the bearings only ship tracking problem - we have working examples in the orgeon library, and it's in the textbooks.

the bearings only problem has some interesting history - it's basically about being on a ww2 era sub. your sub is travelling along, and you begin hearing the sound of a ship at some bearing. over time, as the bearing changes, you can estimate the position and velocity of the ship. keep in mind that both your sub and the ship are moving the entire time. at some point you make a course change for your sub to pursue the ship.

20210308 looking more closely at the oregon library documentation, they have an interesting discussion of objectives - something to think about going forward. is a higher-level statespace model something to consider? putting it this way - adapt/remap a higher-level statespace model into specific lower-level statespace models - we're talking about a translator / converter / adaptor... and we already have an extremely primitive form of that - the rccircuit, jazwinski1, jazwinski2 models are fed into classical, modern, particle. we can think about a higher-level model that can express rccircuit, jazwinski1, and jazwinski2.

designed to allow reuse of a state space definition for state, parameter and joint estimation, using a variety of different inference algorithms. you define your system once in a higher-level state space framework, and then the inference framework generator *geninfds* together with the inference system noise source generator *gensysnoiseds* will adapt/remap that model into a lower-level state space framework needed for whatever type of estimation you want to do. This allows you to focus on defining the model, doing data IO, etc. without having to get bogged down into casting the problem into a different framework each time you want to use a different estimator or want to change the type of inference your doing. I.e. the internal inference implementation is hidden or as transparent as possible with respect to the problem definition by the user.

20210221 brought the documentation via readthedocs up to a minimal level. cleaned up the project and brought some focus to what's going on here. as the docs now make clear - this project focuses on reference problems from Bayesian Signal Processing: Classical, Modern, and Particle Filtering Methods, Kalman Filtering: Theory and Practice, and Stochastic Processes and Filtering Theory - in particular, using numpy for matrix and vector manipulation.

20190310 decision-function-based detector is go. simplest possible case - linear rc-circuit system-model and linear kalman-filter tracker. log-likelihood decision function for detection, ensembles of 100 runs each for signal case and noise case. output curves shown in the first plot - green signal, blue noise-only. roc curves in the second plot. 

<img src="docs/images2/rccircdecfuncs.png" alt=""/>

<img src="docs/images2/rccircroc.png" alt=""/>

20190223

kl-divergence for evaluating sequential monte-carlo - demonstrated below by three pf's in action during the first second of the jazwinksi problem - start-up and convergence. these are 100 hz dist-curves - each dist-curve is a kernel-density-estimate combining hundreds of monte-carlo samples, the fundamental-particles - green dist-curves for truth, blue dist-curves for pf. state-estimates are two red curves on the x,t-plane beneath the dist-curves.

<img src="docs/images2/pf1.png" alt=""/>

<img src="docs/images2/pf2.png" alt=""/>

<img src="docs/images2/pf3.png" alt=""/>

20190105 spkf adaptive jazwinksi switched to square-root filtering, qr-factorization, cholesky-factor update and downdate. improved numerical stability and scaled sampling is clear. still a question around scalar-obs and the obs cholesky-factor and gain. with an adhoc stabilizer on the obs cholesky-factor it's working well overall.

20181230 pf adaptive jazwinksi. parameter-roughening.

20181226 spkf adaptive jazwinski. sample-and-propagate tuning.

20180910 ekf adaptive jazwinski. ud-factorized square-root filtering required for numerical stability.

    