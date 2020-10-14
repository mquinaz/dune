//***************************************************************************
// Copyright 2007-2020 Universidade do Porto - Faculdade de Engenharia      *
// Laboratório de Sistemas e Tecnologia Subaquática (LSTS)                  *
//***************************************************************************
// This file is part of DUNE: Unified Navigation Environment.               *
//                                                                          *
// Commercial Licence Usage                                                 *
// Licencees holding valid commercial DUNE licences may use this file in    *
// accordance with the commercial licence agreement provided with the       *
// Software or, alternatively, in accordance with the terms contained in a  *
// written agreement between you and Faculdade de Engenharia da             *
// Universidade do Porto. For licensing terms, conditions, and further      *
// information contact lsts@fe.up.pt.                                       *
//                                                                          *
// Modified European Union Public Licence - EUPL v.1.1 Usage                *
// Alternatively, this file may be used under the terms of the Modified     *
// EUPL, Version 1.1 only (the "Licence"), appearing in the file LICENCE.md *
// included in the packaging of this file. You may not use this work        *
// except in compliance with the Licence. Unless required by applicable     *
// law or agreed to in writing, software distributed under the Licence is   *
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF     *
// ANY KIND, either express or implied. See the Licence for the specific    *
// language governing permissions and limitations at                        *
// https://github.com/LSTS/dune/blob/master/LICENCE.md and                  *
// http://ec.europa.eu/idabc/eupl.html.                                     *
//***************************************************************************
// Author: Joao Costa                                                       *
//***************************************************************************

// DUNE headers.
#include <DUNE/DUNE.hpp>

namespace Actuators
{
  //! Driver for Doris' Water Sampler 
  //!
  //! This task controls Doris' Water Sampler by asking it to do a sampling procedure of n bottles. 
  //! It adds info about the sampling location and weather conditions to a log and sent back to neptus, as well as the status of the procedures.
  //! @author Joao Costa
  namespace DorisSampler
  {
    using DUNE_NAMESPACES;

    struct Arguments
    {
      //! Serial port device.
      std::string uart_dev;
      //! Serial port baud rate.
      unsigned uart_baud;
      //! Number of bottles to sample
      int bottle_num;
    };

    struct Task: public DUNE::Tasks::Task
    {
      //! Log message.
      //IMC::LogEntry m_logentry;
      //! Serial port handle.
      SerialPort* m_uart;
      //! Task arguments
      Arguments m_args;

      Task(const std::string& name, Tasks::Context& ctx):
        DUNE::Tasks::Task(name, ctx),
        m_uart(NULL)
      {
         param("Serial Port - Device", m_args.uart_dev)
        .defaultValue("/dev/ttyUSB0")
        .description("Serial port device used to communicate with the firmware");

        param("Serial Port - Baud Rate", m_args.uart_baud)
        .defaultValue("57200")
        .description("Serial port baud rate");

        param("Sampler - Bottle Number", m_args.bottle_num)
        .defaultValue("0")
        .minimumValue("0")
        .maximumValue("12")
        .description("Number of bottles to sample");
      }

      //! Update internal state with new parameter values.
      void
      onUpdateParameters(void)
      {
      }

      //! Reserve entity identifiers.
      void
      onEntityReservation(void)
      {
      }

      //! Resolve entity names.
      void
      onEntityResolution(void)
      {
      }

      //! Acquire resources.
      void
      onResourceAcquisition(void)
      {
        m_uart = new SerialPort(m_args.uart_dev, m_args.uart_baud);
        m_uart->setCanonicalInput(true);
      }

      //! Initialize resources.
      void
      onResourceInitialization(void)
      {
        m_uart->writeString("@S,1*\r\n");
        m_uart->writeString("*\r\n");
      }

      //! Release resources.
      void
      onResourceRelease(void)
      {
        Memory::clear(m_uart);
      }

      //! Main loop.
      void
      onMain(void)
      {
        while (!stopping())
        {
          waitForMessages(1.0);
        }
      }
    };
  }
}

DUNE_TASK
