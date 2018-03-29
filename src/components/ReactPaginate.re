[@bs.module]
external reactPaginate : ReasonReact.reactClass = "react-paginate";

type data = {. "selected": int};

let make =
    (
      ~pageCount: int,
      ~pageRangeDisplayed: int,
      ~marginPagesDisplayed: int,
      ~containerClassName="",
      ~pageClassName="",
      ~previousClassName="",
      ~nextClassName="",
      ~previousLinkClassName="",
      ~nextLinkClassName="",
      ~activeClassName="",
      ~pageLinkClassName="",
      ~breakClassName="",
      ~previousLabel=ReasonReact.stringToElement("Previous"),
      ~nextLabel=ReasonReact.stringToElement("Next"),
      ~onPageChange: data => unit=(_) => (),
      children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=reactPaginate,
    ~props={
      "pageCount": pageCount,
      "pageRangeDisplayed": pageRangeDisplayed,
      "marginPagesDisplayed": marginPagesDisplayed,
      "containerClassName": containerClassName,
      "pageClassName": pageClassName,
      "previousClassName": previousClassName,
      "nextClassName": nextClassName,
      "previousLinkClassName": previousLinkClassName,
      "nextLinkClassName": nextLinkClassName,
      "activeClassName": activeClassName,
      "pageLinkClassName": pageLinkClassName,
      "breakClassName": breakClassName,
      "previousLabel": previousLabel,
      "nextLabel": nextLabel,
      "onPageChange": onPageChange,
    },
    children,
  );
