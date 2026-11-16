"""Build the Word, PDF and Markdown reports for Assignment 4 (CO4)."""
from __future__ import annotations

import csv
from pathlib import Path

from docx import Document
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Inches, Pt, RGBColor
from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import inch
from reportlab.platypus import Image, PageBreak, Paragraph, SimpleDocTemplate, Spacer, Table, TableStyle

ROOT = Path(__file__).resolve().parents[1]
REPORT = ROOT / "Report"
REPO_URL = "https://github.com/Hitesh-92007/DSA0101-192524005/tree/main/CO%20ASSIGNMENTS/ASSIGNMENT%204%20%5BCO4%5D"
REPO_LABEL = "DSA0101-192524005 / CO ASSIGNMENTS / ASSIGNMENT 4 [CO4]"

TABLES = {
    "diagnoses": (
        ["Issue", "Effect", "Repair"],
        [
            ["Repeated name and ID", "Fields and maintenance are duplicated", "Move common identity into Staff"],
            ["Public fields", "Callers can create invalid values", "Keep state private"],
            ["Inconsistent names", "nm/code and name/id mean the same thing", "Use one shared naming scheme"],
            ["Unused da member", "Stored state never affects pay", "Remove unused data"],
            ["Magic number 0.42", "Policy is hidden in formula code", "Use a named DA rate constant"],
            ["Different pay interfaces", "One loop cannot process all staff uniformly", "Declare virtual pay() in Staff"],
            ["Adding a staff type", "Clients need category-specific branches", "Add a derived type through the common interface"],
        ],
    ),
    "relationships": (
        ["Relationship", "Kind", "Inheritance form"],
        [
            ["Student derives from Person", "Is-a", "Single"],
            ["Staff derives from Person", "Is-a", "Single"],
            ["TeachingAssistant derives from Student and Staff", "Is-a", "Multiple; hybrid with shared Person"],
            ["ResearchScholar derives from Student", "Is-a", "Multilevel"],
            ["Student stores programme and CGPA", "Has-a/value data", "Member data"],
            ["Staff stores department and salary", "Has-a/value data", "Member data"],
        ],
    ),
    "access": (
        ["Person member", "public inheritance", "protected inheritance", "private inheritance"],
        [
            ["public", "public", "protected", "private"],
            ["protected", "protected", "protected", "private"],
            ["private", "not directly accessible", "not directly accessible", "not directly accessible"],
        ],
    ),
    "dispatch": (
        ["Expression", "Binding rule", "Output"],
        [
            ["p->rate()", "Non-virtual; uses static Account type", "3.5"],
            ["p->vrate()", "Virtual; uses dynamic SeniorAccount type", "4"],
            ["copy.vrate()", "Account value was sliced", "3.5"],
        ],
    ),
    "cart": (
        ["Access style", "Example", "Lifetime / safety"],
        [
            ["Named object", "cart.add(...) / cart.total()", "Automatic scope lifetime; safest for local use"],
            ["Pointer", "pointer->add(...)", "Non-owning; check null and keep target alive"],
            ["Reference", "Cart& ref = cart", "Alias to a live object; cannot be reseated"],
            ["Dynamic pointer", "new Cart / delete", "Manual lifetime; use unique_ptr in production"],
        ],
    ),
    "vehicle_tests": (
        ["Check", "Expected behavior", "Observed result"],
        [
            ["Rent available item", "Positive days mark it rented", "All four standard vehicles rented"],
            ["Reject invalid/already rented request", "Return false", "Guard is implemented in Vehicle::rent"],
            ["Return with two late days", "Charge extended days plus late fee", "330.00, 1000.00, 5800.00, 7116.00"],
            ["List after return", "Status returns to Available", "All four listed as Available"],
            ["Add electric bus", "Use new derived type through Vehicle*", "BUS-01 extension ran; return charge 11075.00"],
        ],
    ),
}

SECTIONS = [
    {
        "title": "Task 1 Class Design Review",
        "paragraphs": [
            "The original payroll classes repeat name and ID, expose their state publicly, use inconsistent names for the same information, retain an unused da field and embed 0.42 as a magic number. Their different data and pay methods also make a common payroll loop difficult.",
            "The repaired abstract Staff class owns name and ID and declares virtual pay(). Professor stores basic pay and research allowance, LabAssistant stores basic pay, and GuestFaculty stores hours and rate. Each derived class overrides the same interface. The payroll vector owns Staff pointers and processes every type in one loop.",
            "For the sample values, Professor Anita earns 81,000.00, LabAssistant Ravi earns 42,600.00, and GuestFaculty Meera earns 30,000.00. A single class with a type switch also has O(1) pay-call time, but it reserves unrelated fields and the new category changes the switch and its type/creation logic. The hierarchy adds a derived class while leaving the existing loop unchanged.",
        ],
        "table": "diagnoses",
        "image": ("Task01/UML.png", "Payroll class UML"),
        "output": ("Task01/output_capture.png", "Task 1 compiled program output"),
    },
    {
        "title": "Task 2 From Requirement to Hierarchy",
        "paragraphs": [
            "Student and Staff are each Persons. TeachingAssistant is both a Student and Staff member, forming a diamond through Person. ResearchScholar is a Student with additional fellowship data. Student and Staff use public virtual inheritance so TeachingAssistant contains one shared Person subobject.",
            "The constructor prediction was Person Student Staff TA. The run matched it. Removing virtual printed Person Student Person Staff TA; this constructor-only program compiles without a diagnostic. Reading ta.name through the non-virtual diamond is ambiguous, and the separate expected-failure compile check records that compiler error.",
            "For composition, TeachingAssistant contains separate Student and Staff objects. Each member has its own Person data, so name access is explicit through student.person or staff.person. On the compiler used, sizeof(Student)=88, sizeof(Staff)=88 and sizeof(TeachingAssistant)=176 bytes. These sizes are ABI-specific. Field lookup is O(1) in both designs.",
        ],
        "table": "relationships",
        "images": [("Task02/UML.png", "People hierarchy UML")],
        "more_tables": ["access"],
        "outputs": [("Task02/output_capture.png", "Task 2 constructor outputs"), ("Task02/EXPECTED_DIAGNOSTIC.txt", "Expected ambiguous-access diagnostic")],
    },
    {
        "title": "Task 3 Constructors and Member Classes",
        "paragraphs": [
            "Address is a value member of Person, and Course is a value member of Student. Constructors pass values through initializer lists. When TeachingAssistant is the most-derived type, its initializer directly constructs the shared virtual Person base; Student and Staff do not construct a second copy.",
            "Observed construction order is virtual bases first, then direct bases in declaration order, then members in declaration order, then the constructor body. Destruction reverses that order. The supplied Engine/Vehicle/Car program printed `Vehicle Engine Car | ~Car ~Engine ~Vehicle`, matching the base/member construction and reverse destruction rules. The table gives the complete logged sequence. The virtual-base TeachingAssistant occupies 24 bytes and the non-virtual comparison occupies 2 bytes on the LLVM-MinGW ABI used; exact sizes vary by implementation.",
            "The completed Student member-class program prints `192311001 Kavya (19) Salem-636001`. Reversing the initializer list produces a -Wreorder-ctor warning, but members still initialize in declaration order and print `1 2`. A pointer-owned Address adds an allocation and ownership work; unique_ptr is safer than raw new/delete, while a value member manages its lifetime automatically.",
        ],
        "image": ("Task03/UML.png", "Member-class and virtual-base UML"),
        "table": "construction",
        "outputs": [("Task03/output_capture.png", "Task 3 constructor, warning and complete-code outputs")],
    },
    {
        "title": "Task 4 Pointers to Derived Classes Forensics",
        "paragraphs": [
            "The defective Account example has a non-virtual rate function, a non-virtual destructor, raw derived storage and a sliced Account value. The rate through Account* is 3.5 because the function is not virtual. Deleting a SeniorAccount through a base pointer is undefined behavior; its derived destructor is skipped on this implementation, leaving its 100-double history allocation outstanding.",
            "The defective source was built with AddressSanitizer. This Windows runtime does not support LeakSanitizer, and it emitted no sanitizer finding in this run. The program's destructor log and explicit allocation counter show two base destructor calls, no SeniorAccount destructor during base-pointer deletion, and one outstanding history buffer. This is direct runtime evidence, not a LeakSanitizer report.",
            "The repair makes rate virtual, uses override, gives Account a virtual destructor and stores owning objects in vector<unique_ptr<Account>>. The corrected run prints 3.5 and 4.0 and releases the senior history. In the required prediction program, the values are 3.5 4 3.5: static call, virtual call, then sliced Account copy. Processing n accounts remains O(n); unique_ptr prevents ownership leaks but does not by itself make a non-virtual destructor safe or stop slicing.",
        ],
        "table": "defects",
        "more_tables": ["dispatch"],
        "outputs": [("Task04/output_capture.png", "Task 4 account outputs"), ("Task04/SANITIZER_RESULTS.txt", "AddressSanitizer build and destructor-tracking record")],
    },
    {
        "title": "Task 5 Pointers to Objects and the this Pointer",
        "paragraphs": [
            "The completed code returns Cart& from add() and setDiscount(), allowing calls to chain on the same object. Inside a member function, this points to the current cart and *this is that cart as a reference. The expression this->discount_ separates the data member from the discount parameter. Returning Cart by value would copy the object and continue the chain on the temporary copy.",
            "The supplied code prints 324.00: 60 + 120 + 180 is 360, and a 10 percent discount leaves 324. The second program demonstrates named-object, pointer, reference and dynamically allocated access. It scans an array of Cart pointers in O(n) time and O(1) extra space, returning the 900.00 cart as the largest.",
            "A vector<Cart> also takes O(n) for the search. It stores full objects; an array of pointers stores pointers while the objects reside elsewhere. A pointer or reference to a vector element can be invalidated when growth reallocates the vector. vector<unique_ptr<Cart>> keeps separately allocated Cart addresses stable as the pointer vector grows.",
        ],
        "table": "cart",
        "outputs": [("Task05/output_capture.png", "Task 5 completed code and pointer-array output")],
    },
    {
        "title": "Task 6 Vehicle Rental Fleet",
        "paragraphs": [
            "Vehicle is abstract and stores registration, daily rate, late-fee rate and rental state. Bicycle derives directly. MotorVehicle adds energy level and a ServiceDate member. TwoWheeler and Car derive from MotorVehicle; Car is abstract and holds seating capacity. FuelCar and ElectricCar implement different energy charges. ElectricBus is added in a separate extension header without changing the existing classes.",
            "Rent accepts a positive day count only when the vehicle is available. Return computes the virtual rentalCharge for booked plus late days, adds a per-day late fee and resets the status. The fleet is an array of Vehicle pointers, so listing and charging use dynamic dispatch. Rental and return work is O(1) per item; listing or processing n fleet entries is O(n). Each vehicle stores a constant number of fields, so its per-object storage is O(1).",
            "With one booked day and two late days, the standard sample totals are Bicycle 330.00, Two-wheeler 1000.00, Car 5800.00 and Electric car 7116.00. The added ElectricBus is listed through Vehicle* and returns for 11075.00 after one late day. A type-code switch also costs O(1) per charge, but adding a type changes the switch and often validation/listing; the hierarchy adds a derived implementation.",
        ],
        "image": ("Task06/UML.png", "Vehicle class hierarchy UML"),
        "table": "vehicle_tests",
        "code": "Task06/pseudocode.md",
        "outputs": [("Task06/output_capture.png", "Task 6 vehicle rental and ElectricBus extension output")],
    },
]

CONSTRUCTION = (
    ["Object", "Construction", "Destruction"],
    [
        ["Student", "Address, Person, Course, Student", "Student, Course, Person, Address"],
        ["Staff", "Address, Person, Staff", "Staff, Person, Address"],
        ["TeachingAssistant", "Address, Person, Course, Student, Staff, TeachingAssistant", "TeachingAssistant, Staff, Student, Course, Person, Address"],
    ],
)

DEFECTS = (
    ["Defect", "Root cause", "Repair"],
    [
        ["Senior rate is not selected", "rate() is non-virtual", "virtual rate() and override"],
        ["Derived cleanup skipped", "Base destructor is non-virtual", "virtual Account destructor"],
        ["History buffer remains allocated", "SeniorAccount destructor does not run through Account*", "virtual destruction and RAII"],
        ["Derived data disappears", "Account copy slices SeniorAccount", "Keep polymorphic objects behind pointers/references"],
        ["Manual ownership is fragile", "Raw owning Account* values", "vector<unique_ptr<Account>>"],
    ],
)

PSEUDOCODE = """PROCEDURE Rent(vehicle, requestedDays)
    IF requestedDays < 1 OR vehicle is already rented THEN
        RETURN false
    END IF
    vehicle.bookedDays <- requestedDays
    vehicle.rented <- true
    RETURN true
END PROCEDURE

PROCEDURE ReturnVehicle(vehicle, lateDays)
    IF vehicle is not rented OR lateDays < 0 THEN
        RETURN invalid-charge
    END IF
    totalDays <- vehicle.bookedDays + lateDays
    total <- vehicle.rentalCharge(totalDays)
    total <- total + lateDays * vehicle.lateFeePerDay
    vehicle.rented <- false
    vehicle.bookedDays <- 0
    RETURN total
END PROCEDURE

PROCEDURE ListFleet(vehiclePointers[0..n-1])
    FOR each pointer in vehiclePointers
        PRINT type, registration, availability, and type-specific details
    END FOR
END PROCEDURE"""


def style_doc(doc: Document):
    section = doc.sections[0]
    section.page_width = Inches(8.27); section.page_height = Inches(11.69)
    section.top_margin = Inches(.7); section.bottom_margin = Inches(.65)
    section.left_margin = Inches(.72); section.right_margin = Inches(.72)
    normal = doc.styles["Normal"]
    normal.font.name = "Aptos"; normal.font.size = Pt(9.5); normal.font.color.rgb = RGBColor(37, 54, 66)
    normal.paragraph_format.space_after = Pt(5)
    for name, size in [("Title", 28), ("Heading 1", 18), ("Heading 2", 12)]:
        s = doc.styles[name]; s.font.name = "Aptos Display"; s.font.size = Pt(size)
        s.font.bold = True; s.font.color.rgb = RGBColor(20, 50, 74)
    footer = section.footer.paragraphs[0]
    footer.alignment = WD_ALIGN_PARAGRAPH.CENTER
    footer.add_run("CO4 Assignment 4 | Hitesh C | 192524005").font.size = Pt(8)


def shade_cell(cell, fill):
    shd = OxmlElement("w:shd"); shd.set(qn("w:fill"), fill); cell._tc.get_or_add_tcPr().append(shd)


def add_docx_table(doc, headers, rows):
    table = doc.add_table(rows=1, cols=len(headers)); table.style = "Table Grid"
    table.autofit = True
    for i, value in enumerate(headers):
        cell = table.rows[0].cells[i]; cell.text = str(value); shade_cell(cell, "17384F")
        for run in cell.paragraphs[0].runs:
            run.bold = True; run.font.color.rgb = RGBColor(255, 255, 255); run.font.size = Pt(8)
    for row in rows:
        cells = table.add_row().cells
        for i, value in enumerate(row):
            cells[i].text = str(value)
            for p in cells[i].paragraphs:
                p.paragraph_format.space_after = Pt(2)
                for run in p.runs: run.font.size = Pt(8)
    doc.add_paragraph()
    return table


def add_docx_image(doc, rel, caption, width=6.6):
    path = ROOT / rel
    if path.exists():
        p = doc.add_paragraph(); p.alignment = WD_ALIGN_PARAGRAPH.CENTER
        p.add_run().add_picture(str(path), width=Inches(width))
        cp = doc.add_paragraph(caption); cp.alignment = WD_ALIGN_PARAGRAPH.CENTER
        for run in cp.runs: run.italic = True; run.font.size = Pt(8)


def markdown_table(headers, rows):
    return "| " + " | ".join(headers) + " |\n|" + "|".join(["---"] * len(headers)) + "|\n" + "\n".join("| " + " | ".join(map(str, r)) + " |" for r in rows)


def build_markdown():
    parts = [
        "# CO4 Assignment 4 Designing Reusable Class Hierarchies", "",
        "**Student:** Hitesh C  ", "**Register number:** 192524005  ",
        "**Course:** Object Oriented Programming with C++ (DSA01)  ",
        "**Repository:** " + REPO_URL, "",
        "This report covers all six tasks: payroll hierarchy design, university relationships, constructor and member-class behavior, polymorphic account defects, Cart pointers and `this`, and a reusable vehicle-rental fleet. The programs were built with C++17 and `-Wall -Wextra`. This folder was prepared with coding assistance. The assignment requires each student to write predictions before running the code and execute every task individually. The included prediction notes are references, not a record of the student's independent predictions. Write your own predictions before rerunning the programs locally, verify the outputs, and add screenshots from those runs. The current output images are formatted from saved logs, not raw terminal screenshots.", "",
        "The available driver is the `g++` command supplied by LLVM-MinGW; `g++ --version` reports Clang 23.1.1. The AddressSanitizer runtime used here does not support LeakSanitizer. The defective account program therefore includes destructor messages and a live-allocation counter; no sanitizer finding was emitted in this run.", "",
    ]
    for section in SECTIONS:
        parts += ["## " + section["title"], ""]
        parts += [p + "\n" for p in section["paragraphs"]]
        for key in [section.get("table"), *(section.get("more_tables") or [])]:
            if not key: continue
            headers, rows = CONSTRUCTION if key == "construction" else DEFECTS if key == "defects" else TABLES[key]
            parts += [markdown_table(headers, rows), ""]
        if section.get("code"):
            parts += ["```text", PSEUDOCODE, "```", ""]
        image = section.get("image")
        if image:
            parts += [f"![{image[1]}]({image[0]})", ""]
        for rel, caption in section.get("images", []):
            parts += [f"![{caption}]({rel})", ""]
        output = section.get("output")
        if output:
            parts += [f"![{output[1]}]({output[0]})", ""]
        for rel, caption in section.get("outputs", []):
            if rel.endswith(".png"):
                parts += [f"![{caption}]({rel})", ""]
            else:
                parts += [f"**{caption}:** see `{rel}`.", ""]
    parts += [
        "## Build and verification summary", "",
        "Seventeen intended programs compiled and ran. The one separate non-virtual `ta.name` source was expected to fail compilation because the name is ambiguous. The initializer-order demonstration produced the expected `-Wreorder-ctor` warning. The Task 3 prediction program printed `Vehicle Engine Car | ~Car ~Engine ~Vehicle`; its completed member-class program printed `192311001 Kavya (19) Salem-636001`. The Task 4 dispatch program printed `3.5 4 3.5`; the completed Task 5 program printed `324.00`.", "",
        "The sanitizer-enabled defective account program exited successfully with leak detection disabled. It printed one outstanding senior history buffer after base-pointer deletion. LeakSanitizer was unavailable in this Windows runtime, so this is not reported as a sanitizer leak finding.", "",
        "Build logs, expected compiler diagnostics, predictions and task outputs are saved in their task folders. The assignment brief leaves its submission date blank. Before submission, write your own predictions, run all programs on your computer, check the results and add raw screenshots from those runs, as required by the assignment.", "",
    ]
    (REPORT / "Report.md").write_text("\n".join(parts), encoding="utf-8")


def build_docx():
    doc = Document(); style_doc(doc)
    p = doc.add_paragraph(style="Title"); p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.add_run("Assignment 4 CO4\nDesigning Reusable Class Hierarchies")
    for line in [
        "Object Oriented Programming with C++ (DSA01)",
        "Individual assignment | 6 tasks | 50 marks",
        "Student: Hitesh C",
        "Register number: 192524005",
        "Repository: " + REPO_LABEL,
        "",
        "Compiler: g++ driver from LLVM-MinGW (reports Clang 23.1.1)",
        "Build standard: C++17 with -Wall -Wextra",
    ]:
        q = doc.add_paragraph(line); q.alignment = WD_ALIGN_PARAGRAPH.CENTER
    doc.add_page_break()
    doc.add_heading("Overview and build method", 1)
    doc.add_paragraph("This report covers all six tasks in Assignment 4: class-hierarchy repair, university people relationships, constructors and member classes, polymorphic account defects, Cart access and the this pointer, and a reusable vehicle-rental fleet. C++ files were compiled with C++17 and -Wall -Wextra, and the programs were run to record their output. This folder was prepared with coding assistance. The assignment requires each student to write predictions before running the code and execute every task individually. The included predictions are references, not a record of the student's independent predictions; write your own before rerunning the programs locally, verify each result and add raw screenshots from those runs.")
    doc.add_paragraph("The available compiler is the g++ command supplied by LLVM-MinGW. Its version output identifies Clang 23.1.1. The source code is compatible with the required C++17 command form. The run logs are in each Task folder. Output images are formatted from the saved run logs and are not live terminal screenshots.")
    doc.add_paragraph("For Task 4, the Windows AddressSanitizer runtime does not support LeakSanitizer. The defective program was built with AddressSanitizer and run with leak detection disabled. No sanitizer finding was emitted; its destructor log and allocation counter show that the base-pointer deletion skipped the SeniorAccount destructor and left one history buffer outstanding.")
    doc.add_heading("Task 1 Class Design Review", 1)
    for p in SECTIONS[0]["paragraphs"]: doc.add_paragraph(p)
    h, rows = TABLES["diagnoses"]; add_docx_table(doc, h, rows)
    add_docx_image(doc, "Task01/UML.png", "Figure 1. Payroll class hierarchy")
    doc.add_page_break()
    doc.add_heading("Task 2 From Requirement to Hierarchy", 1)
    for p in SECTIONS[1]["paragraphs"]: doc.add_paragraph(p)
    add_docx_table(doc, *TABLES["relationships"])
    add_docx_image(doc, "Task02/UML.png", "Figure 2. Virtual-base people hierarchy")
    add_docx_table(doc, *TABLES["access"])
    add_docx_table(doc, *TABLES["dispatch"])
    doc.add_paragraph("Virtual-base output: Person Student Staff TA. Non-virtual output: Person Student Person Staff TA. The constructor-only code compiles in both cases; a separate expected-failure program confirms that ta.name is ambiguous when two Person subobjects exist.")
    add_docx_image(doc, "Task02/output_capture.png", "Task 2 saved output capture")
    doc.add_page_break()
    doc.add_heading("Task 3 Constructors and Member Classes", 1)
    for p in SECTIONS[2]["paragraphs"]: doc.add_paragraph(p)
    add_docx_image(doc, "Task03/UML.png", "Figure 3. Member classes and virtual-base inheritance")
    add_docx_table(doc, *CONSTRUCTION)
    doc.add_paragraph("The supplied Engine, Vehicle and Car sample printed Vehicle Engine Car | ~Car ~Engine ~Vehicle, matching the prediction. Complete-code output: 192311001 Kavya (19) Salem-636001. Reversing the initializer-list order produced a -Wreorder-ctor warning; member declaration order still controlled initialization, and the output was 1 2.")
    add_docx_image(doc, "Task03/output_capture.png", "Task 3 constructor traces and execution evidence")
    doc.add_page_break()
    doc.add_heading("Task 4 Pointers to Derived Classes Forensics", 1)
    for p in SECTIONS[3]["paragraphs"]: doc.add_paragraph(p)
    add_docx_table(doc, *DEFECTS)
    add_docx_table(doc, *TABLES["dispatch"])
    add_docx_image(doc, "Task04/output_capture.png", "Task 4 defective, repaired and prediction outputs")
    doc.add_page_break()
    doc.add_heading("Task 5 Pointers to Objects and the this Pointer", 1)
    for p in SECTIONS[4]["paragraphs"]: doc.add_paragraph(p)
    add_docx_table(doc, *TABLES["cart"])
    add_docx_image(doc, "Task05/output_capture.png", "Task 5 complete-code and search outputs")
    doc.add_page_break()
    doc.add_heading("Task 6 Vehicle Rental Fleet", 1)
    for p in SECTIONS[5]["paragraphs"]: doc.add_paragraph(p)
    add_docx_image(doc, "Task06/UML.png", "Figure 4. Vehicle hierarchy and extension")
    add_docx_table(doc, *TABLES["vehicle_tests"])
    doc.add_heading("Rent and return pseudocode", 2)
    code = doc.add_paragraph(); code.paragraph_format.left_indent = Inches(.25)
    run = code.add_run(PSEUDOCODE); run.font.name = "Consolas"; run.font.size = Pt(8)
    add_docx_image(doc, "Task06/output_capture.png", "Task 6 rental and ElectricBus outputs")
    doc.add_page_break()
    doc.add_heading("Build and verification summary", 1)
    doc.add_paragraph("Seventeen intended programs compiled and ran. The separate non-virtual ta.name example was expected to fail compilation because the name is ambiguous. The initializer-order demonstration emitted the expected reorder warning. The Task 3 prediction program printed Vehicle Engine Car | ~Car ~Engine ~Vehicle, and the completed member-class program printed 192311001 Kavya (19) Salem-636001. The Task 4 dispatch program printed 3.5 4 3.5; and the completed Task 5 program printed 324.00.")
    doc.add_paragraph("AddressSanitizer was enabled for the defective account program, but this Windows runtime does not support leak detection and emitted no sanitizer finding. The program's destructor log and live-allocation counter show one outstanding history buffer after deletion through Account*.")
    doc.add_paragraph("The assignment PDF leaves the submission date blank. Before submission, write your own predictions, execute every task on your computer, check the results and add raw screenshots from those runs. Current output captures are formatted from saved run logs, not raw terminal screenshots.")
    doc.save(REPORT / "CO4_Assignment4_Report.docx")


def pdf_table(headers, data, widths=None):
    table = Table([[Paragraph(str(x), STYLES["TableHead"]) for x in headers]] +
                  [[Paragraph(str(x), STYLES["TableCell"]) for x in row] for row in data],
                  colWidths=widths, repeatRows=1, hAlign="LEFT")
    table.setStyle(TableStyle([
        ("BACKGROUND", (0, 0), (-1, 0), colors.HexColor("#17384F")),
        ("TEXTCOLOR", (0, 0), (-1, 0), colors.white),
        ("GRID", (0, 0), (-1, -1), .35, colors.HexColor("#B5C4CE")),
        ("ROWBACKGROUNDS", (0, 1), (-1, -1), [colors.white, colors.HexColor("#F2F6F8")]),
        ("VALIGN", (0, 0), (-1, -1), "TOP"),
        ("LEFTPADDING", (0, 0), (-1, -1), 5), ("RIGHTPADDING", (0, 0), (-1, -1), 5),
        ("TOPPADDING", (0, 0), (-1, -1), 5), ("BOTTOMPADDING", (0, 0), (-1, -1), 5),
    ]))
    return table


def pdf_image(rel, max_w=7.0*inch, max_h=4.5*inch):
    path = ROOT / rel
    if not path.exists(): return None
    im = Image(str(path)); scale = min(max_w / im.imageWidth, max_h / im.imageHeight)
    im.drawWidth = im.imageWidth * scale; im.drawHeight = im.imageHeight * scale
    im.hAlign = "CENTER"
    return im


def footer(canvas, doc):
    canvas.saveState(); canvas.setFont("Helvetica", 8); canvas.setFillColor(colors.HexColor("#617481"))
    canvas.drawString(.65*inch, .35*inch, "CO4 Assignment 4 | Hitesh C | 192524005")
    canvas.drawRightString(A4[0] - .65*inch, .35*inch, f"Page {doc.page}")
    canvas.restoreState()


def build_pdf():
    global STYLES
    base = getSampleStyleSheet()
    STYLES = {
        "Title": ParagraphStyle("RTitle", parent=base["Title"], fontName="Helvetica-Bold", fontSize=25, leading=30, textColor=colors.HexColor("#14324A"), alignment=TA_CENTER, spaceAfter=12),
        "SubTitle": ParagraphStyle("RSubTitle", parent=base["Normal"], fontName="Helvetica", fontSize=12, leading=17, textColor=colors.HexColor("#39586B"), alignment=TA_CENTER, spaceAfter=8),
        "H1": ParagraphStyle("RH1", parent=base["Heading1"], fontName="Helvetica-Bold", fontSize=16, leading=20, textColor=colors.HexColor("#14324A"), spaceBefore=2, spaceAfter=10),
        "H2": ParagraphStyle("RH2", parent=base["Heading2"], fontName="Helvetica-Bold", fontSize=11, leading=14, textColor=colors.HexColor("#24506A"), spaceBefore=8, spaceAfter=5),
        "Body": ParagraphStyle("RBody", parent=base["BodyText"], fontName="Helvetica", fontSize=9.3, leading=13.4, textColor=colors.HexColor("#253642"), spaceAfter=7),
        "Small": ParagraphStyle("RSmall", parent=base["BodyText"], fontName="Helvetica-Oblique", fontSize=8, leading=10, textColor=colors.HexColor("#536978"), alignment=TA_CENTER, spaceAfter=7),
        "TableHead": ParagraphStyle("RTHead", parent=base["BodyText"], fontName="Helvetica-Bold", fontSize=7.4, leading=9, textColor=colors.white),
        "TableCell": ParagraphStyle("RTCell", parent=base["BodyText"], fontName="Helvetica", fontSize=7.3, leading=9.1, textColor=colors.HexColor("#253642")),
        "Code": ParagraphStyle("RCode", parent=base["Code"], fontName="Courier", fontSize=7.8, leading=10.5, leftIndent=10, rightIndent=8, backColor=colors.HexColor("#F2F6F8"), borderPadding=7, spaceAfter=8),
    }
    story = [Spacer(1, 1.3*inch), Paragraph("Assignment 4 CO4", STYLES["Title"]),
             Paragraph("Designing Reusable Class Hierarchies", STYLES["Title"]),
             Paragraph("Object Oriented Programming with C++ (DSA01)", STYLES["SubTitle"]), Spacer(1, .4*inch),
             Paragraph("Individual assignment | 6 tasks | 50 marks", STYLES["SubTitle"]),
             Paragraph("Student: Hitesh C", STYLES["SubTitle"]),
             Paragraph("Register number: 192524005", STYLES["SubTitle"]),
             Paragraph("Repository: " + REPO_LABEL, STYLES["SubTitle"]), Spacer(1, .4*inch),
             Paragraph("Compiler: g++ driver from LLVM-MinGW (reports Clang 23.1.1)", STYLES["SubTitle"]),
             Paragraph("C++17 | -Wall -Wextra", STYLES["SubTitle"]), PageBreak(),
             Paragraph("Overview and build method", STYLES["H1"]),
             Paragraph("This report covers all six tasks in Assignment 4: class-hierarchy repair, university people relationships, constructors and member classes, polymorphic account defects, Cart access and the this pointer, and a reusable vehicle-rental fleet. C++ files were compiled with C++17 and -Wall -Wextra, and the programs were run to record their output. This folder was prepared with coding assistance. The assignment requires each student to write predictions before running the code and execute every task individually. The included predictions are references, not a record of the student's independent predictions; write your own before rerunning the programs locally, verify each result and add raw screenshots from those runs.", STYLES["Body"]),
             Paragraph("The available compiler is the g++ command supplied by LLVM-MinGW. Its version output identifies Clang 23.1.1. Output images are formatted from saved run logs and are not live terminal screenshots.", STYLES["Body"]),
             Paragraph("The Windows AddressSanitizer runtime does not support LeakSanitizer. The defective account program was built with AddressSanitizer and run with leak detection disabled. No sanitizer finding was emitted; destructor messages and a live-allocation counter show the skipped derived destructor and one outstanding history buffer.", STYLES["Body"])]
    for section in SECTIONS:
        story += [PageBreak(), Paragraph(section["title"], STYLES["H1"])]
        story += [Paragraph(p, STYLES["Body"]) for p in section["paragraphs"]]
        keys = [section.get("table"), *(section.get("more_tables") or [])]
        for key in keys:
            if not key: continue
            headers, rows = CONSTRUCTION if key == "construction" else DEFECTS if key == "defects" else TABLES[key]
            story += [pdf_table(headers, rows), Spacer(1, 8)]
        if section.get("code"):
            story += [Paragraph("Rent and return pseudocode", STYLES["H2"]), Paragraph(PSEUDOCODE.replace("&", "&amp;").replace("<", "&lt;"), STYLES["Code"])]
        image = section.get("image")
        if image:
            flow = pdf_image(image[0])
            if flow: story += [flow, Paragraph(image[1], STYLES["Small"])]
        for rel, caption in section.get("images", []):
            flow = pdf_image(rel)
            if flow: story += [flow, Paragraph(caption, STYLES["Small"])]
        output = section.get("output")
        if output:
            flow = pdf_image(output[0], max_h=4.2*inch)
            if flow: story += [flow]
        for rel, caption in section.get("outputs", []):
            if rel.endswith(".png"):
                flow = pdf_image(rel, max_h=4.2*inch)
                if flow: story += [flow]
    story += [PageBreak(), Paragraph("Build and verification summary", STYLES["H1"]),
              Paragraph("Seventeen intended programs compiled and ran. The separate non-virtual ta.name example was expected to fail compilation because the name is ambiguous. The initializer-order demonstration emitted the expected reorder warning. Task 3 predicted output was Vehicle Engine Car | ~Car ~Engine ~Vehicle. Completed code outputs were 192311001 Kavya (19) Salem-636001, 3.5 4 3.5, and 324.00 for Tasks 3, 4 and 5 respectively.", STYLES["Body"]),
              Paragraph("The AddressSanitizer run used leak detection disabled because the Windows runtime does not support LeakSanitizer. It emitted no sanitizer finding. The tracked history buffer and destructor output demonstrate the skipped derived cleanup; this is not presented as a LeakSanitizer report.", STYLES["Body"]),
              Paragraph("The assignment PDF leaves the submission date blank. Before submission, write your own predictions, execute every task on your computer, check the results and add raw screenshots from those runs. Current output captures are formatted from saved run logs, not raw terminal screenshots.", STYLES["Body"])]
    pdf = SimpleDocTemplate(str(REPORT / "CO4_Assignment4_Report.pdf"), pagesize=A4,
                            leftMargin=.62*inch, rightMargin=.62*inch,
                            topMargin=.58*inch, bottomMargin=.62*inch,
                            title="CO4 Assignment 4 Designing Reusable Class Hierarchies", author="Hitesh C")
    pdf.build(story, onFirstPage=footer, onLaterPages=footer)


def main():
    REPORT.mkdir(parents=True, exist_ok=True)
    build_markdown(); build_docx(); build_pdf()
    print("Built Markdown, Word and PDF reports.")


if __name__ == "__main__":
    main()
