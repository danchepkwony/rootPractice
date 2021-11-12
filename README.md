# Methods

I took [this dataset](https://archive.ics.uci.edu/ml/datasets/student+performance) and parsed the csv file so it could be read by Root.

It contains various data of students in a math class, including their final grade on a 20 point scale.

If their final grade was greater than or equal to 10.8, the grade was passing. Passing grades were signal events while failing grades were background.

I attempted to use other variables: study time, past failures, health, absences, school, and first term grade (G1) to predict if the student would pass or fail.

## Input variables

![Input variables](./inputVariables.png)

### Study Time:

<ul>
    <li>1: Less than 2 hours</li>
    <li>2: 2 - 5 hours</li>
    <li>3: 5 - 10 hours</li>
    <li>4: More than 10 hours</li>
</ul>

### Past Failures: 

<ul>
    <li>n if 1 ≤ n < 3, else 4</li>
</ul>

### Health: 

<ul>
    <li>1: Very Bad</li>
    <li>2: Bad</li>
    <li>3: Ok</li>
    <li>4: Good</li>
    <li>5: Very Good</li>
</ul>

### Absences: 

<ul>
    <li>0 - 93</li>
</ul>

### School: 

<ul>
    <li>0: Mousinho da Silveira</li>
    <li>1: Gabriel Pereira</li>
</ul>

### G1:

<ul>
    <li>0 - 20</li>
</ul>

# Results

#### Results Without G1

![Results without G1](./withoutG1.png)

<em>The red-dashed line represents the ROC curve of a random classifier.
The classifier is slightly better than random</em>

#### Results With G1

![Results with G1](./withG1.png)
