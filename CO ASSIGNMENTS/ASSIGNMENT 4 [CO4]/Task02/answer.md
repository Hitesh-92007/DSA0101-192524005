# Task 2 From Requirement to Hierarchy

## 2.1 Relationship classification

| Relationship | Kind | Inheritance form |
|---|---|---|
| Student is a Person | Is-a | Single |
| Staff is a Person | Is-a | Single |
| TeachingAssistant is a Student and Staff | Is-a | Multiple; hybrid with the shared Person base |
| ResearchScholar is a Student | Is-a | Multilevel |
| Student has programme information | Has-a/value data | Composition/value member |
| Staff has department and salary information | Has-a/value data | Composition/value member |

The diamond is `Person -> Student -> TeachingAssistant` and `Person -> Staff -> TeachingAssistant`. With ordinary inheritance, the teaching assistant contains two `Person` subobjects. Virtual inheritance makes the two paths share one virtual `Person` base.

## 2.2 and 2.3 Virtual-base design

`Student` and `Staff` use `public virtual Person`. Public inheritance preserves the public interface of the base for an is-a relationship. `TeachingAssistant` inherits publicly from both and, as the most-derived class, initializes the one shared `Person` base. `ResearchScholar` publicly derives from `Student` and adds its fellowship value.

```text
                         Person
                    /             \\
        virtual public             virtual public
               Student             Staff
                    \\             /
                     TeachingAssistant

Person -> Student -> ResearchScholar
```

## 2.4 Access table

| Member access in Person | Public inheritance in Student | Protected inheritance | Private inheritance |
|---|---|---|---|
| `public` | public | protected | private |
| `protected` | protected | protected | private |
| `private` | not directly accessible | not directly accessible | not directly accessible |

## Predict the output

The initial prediction and the constructor-only output are in `PREDICTION.md` and `RUN_OUTPUT.txt`. The virtual-base program prints the `Person` constructor once. Removing `virtual` prints it twice. The compiler gives no diagnostic for constructing duplicate bases; a separate source that reads `ta.name` fails because both base paths provide a `Person::name` member.

## Composition alternative

With `TeachingAssistant { Student student; Staff staff; }`, the object has-a student and has-a staff. Each member is a distinct complete object with its own Person data, so there are two Person records. Member access is explicit (`ta.student.person.name` or `ta.staff.person.name`) and is not ambiguous. Inheritance gives the TA is-a semantics and a shared Person via virtual inheritance. Both alternatives use O(1) access time for the named fields; object size depends on implementation, string representation, and padding. The source prints compiler-specific `sizeof` values rather than claiming a universal byte count.

## Research scholar

`ResearchScholar` is declared in the complete hierarchy diagram and answer: it derives from `Student` and adds fellowship data, which is a multilevel relationship. The constructor trace is shown in Task 3.
