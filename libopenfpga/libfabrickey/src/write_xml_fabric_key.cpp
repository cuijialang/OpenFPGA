/********************************************************************
 * This file includes functions that outputs a configuration protocol to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>
#include <algorithm>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"
#include "openfpga_digest.h"

/* Headers from arch openfpga library */
#include "write_xml_utils.h" 

/* Headers from fabrickey library */
#include "write_xml_fabric_key.h"

/********************************************************************
 * A writer to output a component key to XML format
 *******************************************************************/
static 
void write_xml_fabric_component_key(std::fstream& fp,
                                    const char* fname,
                                    const FabricKey& fabric_key,
                                    const FabricKeyId& component_key) {
  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  fp << "\t" << "<key";

  write_xml_attribute(fp, "id", size_t(component_key));
  write_xml_attribute(fp, "name", fabric_key.key_name(component_key).c_str());
  write_xml_attribute(fp, "value", fabric_key.key_value(component_key));

  fp << "/>" << "\n";
}

/********************************************************************
 * A writer to output a fabric key to XML format
 *******************************************************************/
void write_xml_fabric_key(const char* fname,
                          const FabricKey& fabric_key) {

  vtr::ScopedStartFinishTimer timer("Write Fabric Key");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "<fabric_key>" << "\n";

  /* Write component by component */ 
  for (const FabricKeyId& key : fabric_key.keys()) {
    write_xml_fabric_component_key(fp, fname, fabric_key, key);
  }

  /* Finish writing the root node */
  fp << "</fabric_key>" << "\n";

  /* Close the file stream */
  fp.close();
}
