#include "srs.h"

/**
 * add_course - add a new course to the courses linked list
 * @list: pointer to head of courses linked list
 * @first_name: first name of course
 * @last_name: last name of course
 */
int add_course(course_t **list, int roll_number, char *course_name, int score, int silent)
{
  course *new_course = (course *)malloc(sizeof(course));
  if (!new_course)
  {
    printf("Could not allocate enough memory for new course\n");
    return EXIT_FAILURE;
  }

  new_course->roll_number = roll_number;
  new_course->course_name = strdup(course_name);
  new_course->score = score;

  course_t *new_node = (course_t *)malloc(sizeof(course_t));
  if (!new_node)
  {
    free(new_course->course_name);
    free(new_course);
    printf("Could not allocate enough memory for new course\n");
    return EXIT_FAILURE;
  }

  new_node->data = new_course;

  if (*list == NULL)
    *list = new_node;
  else
  {
    course_t *current = *list;
    while (current->next)
      current = current->next;
    current->next = new_node;
    new_node->prev = current;
  }
  if (!silent)
    printf("Course added: (%d). %s %d\n", new_course->roll_number, course_name, score);
  return EXIT_SUCCESS;
}

/**
 * print_courses - print courses in courses linked list
 * @list: point to the head of the linked list
 */
void print_courses(course_t *list)
{
  course_t *current = list;

  printf("%-15s %-25s %-25s\n", "Roll Number", "Course Name", "Score");
  printf("%-15s %-25s %-25s\n", "------------", "----------", "---------");

  while (current)
  {
    printf("%-15d %-25s %-25d\n", current->data->roll_number, current->data->course_name, current->data->score);
    current = current->next;
  }
}

/**
 * save_to_csv - save list of courses to CSV file
 * @list: pointer to linked list head
 * @filename: name of csv file
 */
void save_courses_to_csv(course_t *list, char *filename)
{
  FILE *file = fopen(filename, "w");
  if (!file)
    return perror("Could not open file");

  course_t *current = list;

  fprintf(file, "Roll Number, Course Name, Score\n");
  while (current)
  {
    fprintf(file, "%d, %s, %d\n", current->data->roll_number, current->data->course_name, current->data->score);
    current = current->next;
  }

  fclose(file);
}

/**
 * load_from_csv - load list of courses from CSV file
 * @list: pointer to linked list head
 * @filename: name of csv file
 */
void load_courses_from_csv(course_t **list, char *filename)
{
  FILE *file = fopen(filename, "r");
  if (!file)
    return perror("Could not open file");

  char line[256];
  fgets(line, sizeof(line), file);

  while (fgets(line, sizeof(line), file))
  {
    line[strcspn(line, "\n")] = 0;

    int roll_number;
    char course_name[50];
    int score;

    if (sscanf(line, "%d,%49[^,],%d", &roll_number, course_name, &score) != 3)
    {
      fprintf(stderr, "Could not parse line: %s\n", line);
      return;
    }

    add_course(list, roll_number, course_name, score, 1);
  }

  fclose(file);
}

/**
 * show_course_help - display help page for course command
 */
void show_course_help(void)
{
  printf("\nNAME\n");
  printf("    course - Manage students' courses interactively\n");

  printf("\nSYNOPSIS\n");
  printf("    course [COMMAND] [OPTIONS]\n");

  printf("\nDESCRIPTION\n");
  printf("    This program allows you to manage students' courses. ");
  printf("You can add, remove, display, list, sort, and search for students' courses. ");
  printf("The program runs in an infinite loop until 'exit' is entered.\n");

  printf("\nCOMMANDS\n");
  printf("    course add <roll_number>\n");
  printf("        Add a new course for a student. You will be prompted for the course name and the score.\n\n");

  printf("    course remove <roll_number> <course_name>\n");
  printf("        Remove a course for the specified student by roll number and course name.\n\n");

  printf("    course show <roll_number>\n");
  printf("        Display all the courses and scores for the specified student by roll number.\n\n");

  printf("    course list\n");
  printf("        List all students with their courses.\n\n");

  printf("    course sort asc|desc\n");
  printf("        Sort all students and their courses by roll number in ascending or descending order.\n\n");

  printf("    course find <query>\n");
  printf("        Find a course by its name. The <query> can be any part of the course name.\n\n");

  printf("    course save <filename>\n");
  printf("        Save list of courses to a text/csv file.\n\n");

  printf("    course load <filename>\n");
  printf("        Load list of courses from a text/csv file.\n\n");

  printf("    course help\n");
  printf("        Display this help information for course commands.\n\n");

  printf("\nEXAMPLES\n");
  printf("    course add 1\n");
  printf("        Prompts for a course name and score, then adds the course to the student with roll number 1.\n\n");

  printf("    course show 2\n");
  printf("        Displays all courses and their scores for the student with roll number 2.\n\n");

  printf("    exit\n");
  printf("        Exits the program.\n");
}
